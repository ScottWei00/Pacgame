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
	class USphereComponent* PacdotCollision;//声明糖豆人食物的碰撞检测组件

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PacdotDisplayMesh;//声明糖豆人食物的形状静态组件

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSuperPacdot=false;//是否是特殊食物

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
