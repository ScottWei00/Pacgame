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

	//������ȡ���������е��Ƕ�����
	for (TActorIterator<APacdot> PacdotItr(GetWorld()); PacdotItr; ++PacdotItr)
	{
		PacdotNum++;
	}

	//������ȡ���еĵ���
	for (TActorIterator<APacmanEnemy> EnemyItr(GetWorld()); EnemyItr; ++EnemyItr)
	{
		Enemies.Add(Cast<APacmanEnemy>(*EnemyItr));
	}


}


//��ʼ��Ϸģʽ��ֻ�е�ǰ���ڲ˵�ģʽ�ǲ��ܽ��뿪ʼģʽ���ó��������еĵ��˽����ƶ�
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

//��ͣ��Ϸ�ڿ�ʼ����ͣ���ֻ�
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

//���¿�ʼ��Ϸ
void APacgameGameModeBase::RestartGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand(TEXT("RestartLevel"));
}

//��player�Ե�SuperDot��ʱ���������е��˴���
void APacgameGameModeBase::SetEnemiesVulnerable()
{
	for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
	{
		(*Iter)->SetVulnerable();
	}
}

//������Ϸ��ͣʱ��¼��Ϸ�ĸ������˵ļ�ʱ��
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

//����Ϸ����ͣתΪ����ʱ���ָ���ͣ��ʱ��
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
		//��ʼ��Ϸ�����е��˺�player������
		case EGameState::EPlay:
			UE_LOG(LogTemp, Warning, TEXT("play"));
			for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
			{
				(*Iter)->SetMovement(true);
			}
			Cast<APacmanPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn())->SetMovement(true);
			break; 
		//ʤ�������е��˱��ݻ�
		case EGameState::EWin:
			UE_LOG(LogTemp, Warning, TEXT("Win"));
			for (auto Iter(Enemies.CreateConstIterator()); Iter; ++Iter)
			{
				(*Iter)->Destroy();
			}
			break;
		//��ͣ����Ϸ���������е��˺�player��ֹ����
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