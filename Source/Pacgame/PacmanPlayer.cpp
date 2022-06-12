// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Pacdot.h"

// Sets default values 构造函数
APacmanPlayer::APacmanPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Velocity = FVector(0, 0, 0);//初始化速度大小

	PacmanPlayerDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanPlayerDisplayMesh"));//创建声明糖豆人显示组件

	PacmanPlayerDisplayMesh->SetupAttachment(GetCapsuleComponent());//绑定到character自带的胶囊碰撞体上

}

// Called when the game starts or when spawned 游戏开始先运行begin play并且只会运行一次
void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//注意：碰撞检测方面的逻辑只能写在BeginPlay，不能写在构造函数里，不然引擎会出错
	//将人物character胶囊体组件和碰撞检测函数进行绑定
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanPlayer::OnPacmanBeginOverlay);
}

// Called every frame
void APacmanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacmanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &APacmanPlayer::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APacmanPlayer::MoveY);
}

void APacmanPlayer::MoveX(float value)
{
	Velocity.X = value;
	Velocity.Y = 0;

	if (value > 0)
	{
		SetActorRotation(FRotator(0,0,0));//更改character物体的位置，旋转
	}
	else if (value < 0) 
	{
		SetActorRotation(FRotator(0,180,0));//注意Frotator里的参数是(Pitch, Yaw, Row)对应(y, z, x)
	}
	AddMovementInput(Velocity);
}

void APacmanPlayer::MoveY(float value)
{
	Velocity.Y = value;
	Velocity.X = 0;

	//移动方向改了，人物的朝向也需要相应更改
	if (value > 0)
	{
		SetActorRotation(FRotator(0, 90, 0));//更改character物体的位置，旋转
	}
	else if (value < 0)
	{
		SetActorRotation(FRotator(0, -90, 0));
	}
	AddMovementInput(Velocity);//更新人物移动速度和方向
}

void APacmanPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//判断和character人物胶囊体发生碰撞的是不是糖豆人里的实物,如果是，则删除该实物
	if (OtherActor->IsA(APacdot::StaticClass()))
	{
		OtherActor->Destroy();
	}

}

