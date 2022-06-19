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
	// Sets default values for this character's properties���캯��
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
	EGameState CurrentState;//��¼��ǰ����Ϸģʽ״̬

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PacdotNum;//��¼���Ե����Ƕ�������
 
private:
	TArray<class APacmanEnemy*> Enemies;//�洢���еĵ���
	TArray<float> EnemiesTimerRemaining;//ÿ�����˵ļ�ʱ��
};
 

FORCEINLINE EGameState APacgameGameModeBase::GetGameStata() const
{
	return CurrentState;
}



FORCEINLINE int APacgameGameModeBase::GetPacdotNum() const
{
	return PacdotNum;
}

