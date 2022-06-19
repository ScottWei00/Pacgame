// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PacmanPlayer.generated.h"

UCLASS()
class PACGAME_API APacmanPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacmanPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacmanPlayerDisplayMesh;//声明糖豆人显示组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Lifes;//pacman的生命值

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float NormalSpeed;//移动速度

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvincibleTime;//无敌时间

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input 按键绑定函数
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//player可能会发生的一些事情函数
	void Injured();
	void Resetting();
	void SetMovement(bool bCanMove);

	//游戏模式类型：开始游戏、暂停游戏、重新游戏
	void StartGame();
	void PauseGame();
	void RestartGame();

	//游戏人物移动：x、y轴移动
	void MoveX(float value);
	void MoveY(float value);

	//糖豆人和食物的碰撞检测
	UFUNCTION()
	void OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector Velocity;
	class APacgameGameModeBase* GameModeRef;

	FVector StartLocation;

	FTimerHandle ResettingTimerHandle;//计时器
 
};
