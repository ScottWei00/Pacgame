// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacgameGameModeBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameState :uint8
{
	EMenu,
	EPlay,
	EPause,
	EWin,
	EGameover
};

UCLASS()
class PACGAME_API APacgameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties构造函数
	APacgameGameModeBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	void StartGame();
	void PauseGame();
	void RestartGame();
	void SetEnemiesVulnerable();

	void RecordEnemiesTimer();
	void RestoreEnemiesTimer();

	EGameState GetGameStata() const;
	void SetGameState(EGameState state);
 
	int GetPacdotNum() const;
	void SetPacdotNum(int value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EGameState CurrentState;//记录当前的游戏模式状态

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PacdotNum;//记录被吃掉的糖豆的数量
 
private:
	TArray<class APacmanEnemy*> Enemies;//存储所有的敌人
	TArray<float> EnemiesTimerRemaining;//每个敌人的计时器
};
 

FORCEINLINE EGameState APacgameGameModeBase::GetGameStata() const
{
	return CurrentState;
}



FORCEINLINE int APacgameGameModeBase::GetPacdotNum() const
{
	return PacdotNum;
}

