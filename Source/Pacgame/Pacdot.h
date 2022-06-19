// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pacdot.generated.h"

UCLASS(Blueprintable)
class PACGAME_API APacdot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APacdot();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USphereComponent* PacdotCollision;//�����Ƕ���ʳ�����ײ������

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacdotDisplayMesh;//�����Ƕ���ʳ�����״��̬���

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSuperPacdot=false;//�Ƿ�������ʳ��

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
