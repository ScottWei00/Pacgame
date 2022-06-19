// Copyright Epic Games, Inc. All Rights Reserved.


#include "PacgameGameModeBase.h"
#include "Pacdot.h"
#include "PacmanEnemy.h"
#include "PacmanPlayer.h"
#include "Engine/Public/EngineUtils.h"
#include "EnemyController.h"

APacgameGameModeBase::APacgameGameModeBase()
{
	CurrentState = EGameState::EMenu;
}

void APacgameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//遍历获取场景中所有的糖豆数量
	for (TActorIterator<APacdot> PacdotItr(GetWorld()); PacdotItr; ++PacdotItr)
	{
		PacdotNum++;
	}

	//遍历获取所有的敌人
	for (TActorIterator<APacmanEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
	{
		Enemies.Add(Cast<APacmanEnemy>(*EnemyItr));
	}


}


//开始游戏模式：只有当前处于菜单模式是才能进入开始模式，让场景中所有的敌人进行移动
void APacgameGameModeBase::StartGame()
{
	if (CurrentState == EGameState::EMenu)
	{
		SetGameState(EGameState::EPlay);
		for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
		{
			Cast<AEnemyController>((*Iter)->GetController())->GotoNewDestination();
		}
	}
}

//暂停游戏在开始和暂停中轮换
void APacgameGameModeBase::PauseGame()
{
	if (CurrentState == EGameState::EPlay)
	{
		RecordEnemiesTimer();
		SetGameState(EGameState::EPause);
	}
	else if (CurrentState == EGameState::EPause)
	{
		RestoreEnemiesTimer();
		SetGameState(EGameState::EPlay);
	}
}

//重新开始游戏
void APacgameGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

//当player吃到SuperDot的时候，设置所有敌人脆弱
void APacgameGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		(*Iter)->SetVulnerable();
	}
}

//用于游戏暂停时记录游戏的各个敌人的计时器
void APacgameGameModeBase::RecordEnemiesTimer()
{
	EnemiesTimerRemaining.Empty();

	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		if ((*Iter)->bIsVulnerable == false && (*Iter)->bIsDead == false)
		{
			EnemiesTimerRemaining.Add(-1);
		}
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)
		{
			float VulnerableTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->VulnerableTimerHandle);
			EnemiesTimerRemaining.Add(VulnerableTimerRemaining);
		}
		if ((*Iter)->bIsDead == true)
		{
			float DeadTimerRemaining = GetWorldTimerManager().GetTimerRemaining((*Iter)->DeadTimerHandle);
			EnemiesTimerRemaining.Add(DeadTimerRemaining);
		}
	}
}

//在游戏由暂停转为继续时，恢复暂停的时间
void APacgameGameModeBase::RestoreEnemiesTimer()
{
	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		if ((*Iter)->bIsVulnerable == true && (*Iter)->bIsDead == false)
		{
			(*Iter)->SetVulnerable(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
		if ((*Iter)->bIsDead == true)
		{
			(*Iter)->SetDead(true, EnemiesTimerRemaining[Iter.GetIndex()]);
		}
	}
}


void APacgameGameModeBase::SetGameState(EGameState state)
{
	CurrentState = state;
	switch (CurrentState)
	{ 
		//开始游戏，所有敌人和player动起来
		case EGameState::EPlay:
			UE_LOG(LogTemp, Warning, TEXT("play"));
			for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
			{
				(*Iter)->SetMovement(true);
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);
			break; 
		//胜利，所有敌人被摧毁
		case EGameState::EWin:
			UE_LOG(LogTemp, Warning, TEXT("Win"));
			for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
			{
				(*Iter)->Destroy();
			}
			break;
		//暂停和游戏结束：所有敌人和player禁止不动
		case EGameState::EPause:
			UE_LOG(LogTemp, Warning, TEXT("pause"));
			for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
			{
				(*Iter)->SetMovement(false);
				GetWorldTimerManager().ClearAllTimersForObject(*Iter);
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);
			break;
		case EGameState::EGameover: 
			UE_LOG(LogTemp, Warning, TEXT("GameOver"));
			for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
			{
				(*Iter)->SetMovement(false); 
				GetWorldTimerManager().ClearAllTimersForObject(*Iter);
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(false);
			break;
		default:
			break;
	}
}


void APacgameGameModeBase::SetPacdotNum(int value)
{
	PacdotNum = value;
	if (value == 0)
	{
		SetGameState(EGameState::EWin);
	}
}