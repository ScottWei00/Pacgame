// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PacmanEnemy.generated.h"

UCLASS()
class PACGAME_API APacmanEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacmanEnemy();

	//���������������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacmanEnemyDisplayMesh;

	bool bIsDead;
	bool bIsVulnerable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeadTime;//����ʱ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableTime;//����ʱ��
	
	FTimerHandle DeadTimerHandle;
	FTimerHandle VulnerableTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalSpeed;//�ƶ��ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VulnerableSpeed; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetVulnerable(bool bRestoreTimer = false, float Time = 0.0f);
	void SetDead(bool bRestoreTimer = false, float Time = 0.0f);
	void SetNormal();
	void SetMovement(bool bCanMove);

	//enemy��player������ײ
	UFUNCTION()
	void OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:
	FVector StartLocation;
};
