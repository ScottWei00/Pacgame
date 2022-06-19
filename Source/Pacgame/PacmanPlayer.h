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
	class UStaticMeshComponent* PacmanPlayerDisplayMesh;//�����Ƕ�����ʾ���

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Lifes;//pacman������ֵ

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float NormalSpeed;//�ƶ��ٶ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InvincibleTime;//�޵�ʱ��

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input �����󶨺���
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//player���ܻᷢ����һЩ���麯��
	void Injured();
	void Resetting();
	void SetMovement(bool bCanMove);

	//��Ϸģʽ���ͣ���ʼ��Ϸ����ͣ��Ϸ��������Ϸ
	void StartGame();
	void PauseGame();
	void RestartGame();

	//��Ϸ�����ƶ���x��y���ƶ�
	void MoveX(float value);
	void MoveY(float value);

	//�Ƕ��˺�ʳ�����ײ���
	UFUNCTION()
	void OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector Velocity;
	class APacgameGameModeBase* GameModeRef;

	FVector StartLocation;

	FTimerHandle ResettingTimerHandle;//��ʱ��
 
};
