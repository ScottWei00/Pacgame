// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Pacdot.h"
#include "PacgameGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Public/TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values 构造函数
APacmanPlayer::APacmanPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Velocity = FVector(0, 0, 0);//初始化速度大小

	Lifes = 3;//初始生命值为3
	NormalSpeed = 600.f;//初始速度
	InvincibleTime = 3.f;//无敌时间

	PacmanPlayerDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanPlayerDisplayMesh"));//创建声明糖豆人显示组件

	PacmanPlayerDisplayMesh->SetupAttachment(GetCapsuleComponent());//绑定到character自带的胶囊碰撞体上

}

// Called when the game starts or when spawned 游戏开始先运行begin play并且只会运行一次
void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();

	//记录pacman起始位置
	StartLocation = GetActorLocation();

	//将游戏模式转换为我们创建的游戏模式并赋值给GameModeRef
	GameModeRef = Cast<APacgameGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	//注意：碰撞检测方面的逻辑只能写在BeginPlay，不能写在构造函数里，不然引擎会出错
	//将人物character胶囊体组件和碰撞检测函数进行绑定
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanPlayer::OnPacmanBeginOverlay);

	SetMovement(true);
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

	PlayerInputComponent->BindAction("StartGame", IE_Pressed, this, &APacmanPlayer::StartGame);
	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &APacmanPlayer::PauseGame);
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &APacmanPlayer::RestartGame);

	PlayerInputComponent->BindAxis("MoveX", this, &APacmanPlayer::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APacmanPlayer::MoveY);
}

//更新player的受伤处理
void APacmanPlayer::Injured()
{
	//防止enemy和player进行一瞬间多次碰撞接触，所以发生一次碰撞就更改其碰撞预设，保证3s无敌时间
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));
	Lifes--; 
	if (Lifes == 0)
	{
		GameModeRef->SetGameState(EGameState::EGameover);
	}
	else
	{
		SetActorLocation(StartLocation);
		//定时器，InvincibleTime后即可跳到Resetting函数
		GetWorld()->GetTimerManager().SetTimer(ResettingTimerHandle, this, &APacmanPlayer::Resetting, InvincibleTime, false);
	}
}

void APacmanPlayer::Resetting()
{
	//3s无敌时间结束后，清除计时器
	GetWorld()->GetTimerManager().ClearTimer(ResettingTimerHandle);
	//设置回原来的碰撞预设
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

//更新player的移动速度
void APacmanPlayer::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void APacmanPlayer::StartGame()
{
	GameModeRef->StartGame();
}

void APacmanPlayer::PauseGame()
{
	GameModeRef->PauseGame();
}

void APacmanPlayer::RestartGame()
{
	GameModeRef->RestartGame();
}

void APacmanPlayer::MoveX(float value)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
	{
		Velocity.X = value;
		Velocity.Y = 0;

		if (value > 0)
		{
			SetActorRotation(FRotator(0, 0, 0));//更改character物体的位置，旋转
		}
		else if (value < 0)
		{
			SetActorRotation(FRotator(0, 180, 0));//注意Frotator里的参数是(Pitch, Yaw, Row)对应(y, z, x)
		}
		AddMovementInput(Velocity);
	}
}

void APacmanPlayer::MoveY(float value)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
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
}

void APacmanPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
	{
		//判断和character人物胶囊体发生碰撞的是不是糖豆人里的实物,如果是，则删除该实物
		if (OtherActor->IsA(APacdot::StaticClass()))
		{
			APacdot* CurrentPacdot = Cast<APacdot>(OtherActor);
			if (CurrentPacdot->IsSuperPacdot)
			{
				GameModeRef->SetEnemiesVulnerable();
			}
			OtherActor->Destroy();
			GameModeRef->SetPacdotNum(GameModeRef->GetPacdotNum() - 1);
		}
	}
}

