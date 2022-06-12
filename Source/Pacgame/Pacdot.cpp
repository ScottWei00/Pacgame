// Fill out your copyright notice in the Description page of Project Settings.


#include "Pacdot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APacdot::APacdot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PacdotCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PacdotCollision"));//创建食物碰撞体的实例
	PacdotDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacdotDisplayMesh"));//创建食物的形状实例

	PacdotDisplayMesh->SetupAttachment(PacdotCollision);//将食物的物体绑定到碰撞体上，便于识别和碰撞检测
}

// Called when the game starts or when spawned
void APacdot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APacdot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

