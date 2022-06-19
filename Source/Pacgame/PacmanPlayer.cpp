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

// Sets default values ���캯��
APacmanPlayer::APacmanPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Velocity = FVector(0, 0, 0);//��ʼ���ٶȴ�С

	Lifes = 3;//��ʼ����ֵΪ3
	NormalSpeed = 600.f;//��ʼ�ٶ�
	InvincibleTime = 3.f;//�޵�ʱ��

	PacmanPlayerDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanPlayerDisplayMesh"));//���������Ƕ�����ʾ���

	PacmanPlayerDisplayMesh->SetupAttachment(GetCapsuleComponent());//�󶨵�character�Դ��Ľ�����ײ����

}

// Called when the game starts or when spawned ��Ϸ��ʼ������begin play����ֻ������һ��
void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();

	//��¼pacman��ʼλ��
	StartLocation = GetActorLocation();

	//����Ϸģʽת��Ϊ���Ǵ�������Ϸģʽ����ֵ��GameModeRef
	GameModeRef = Cast<APacgameGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	//ע�⣺��ײ��ⷽ����߼�ֻ��д��BeginPlay������д�ڹ��캯�����Ȼ��������
	//������character�������������ײ��⺯�����а�
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

//����player�����˴���
void APacmanPlayer::Injured()
{
	//��ֹenemy��player����һ˲������ײ�Ӵ������Է���һ����ײ�͸�������ײԤ�裬��֤3s�޵�ʱ��
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Invincible"));
	Lifes--; 
	if (Lifes == 0)
	{
		GameModeRef->SetGameState(EGameState::EGameover);
	}
	else
	{
		SetActorLocation(StartLocation);
		//��ʱ����InvincibleTime�󼴿�����Resetting����
		GetWorld()->GetTimerManager().SetTimer(ResettingTimerHandle, this, &APacmanPlayer::Resetting, InvincibleTime, false);
	}
}

void APacmanPlayer::Resetting()
{
	//3s�޵�ʱ������������ʱ��
	GetWorld()->GetTimerManager().ClearTimer(ResettingTimerHandle);
	//���û�ԭ������ײԤ��
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
}

//����player���ƶ��ٶ�
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
			SetActorRotation(FRotator(0, 0, 0));//����character�����λ�ã���ת
		}
		else if (value < 0)
		{
			SetActorRotation(FRotator(0, 180, 0));//ע��Frotator��Ĳ�����(Pitch, Yaw, Row)��Ӧ(y, z, x)
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

		//�ƶ�������ˣ�����ĳ���Ҳ��Ҫ��Ӧ����
		if (value > 0)
		{
			SetActorRotation(FRotator(0, 90, 0));//����character�����λ�ã���ת
		}
		else if (value < 0)
		{
			SetActorRotation(FRotator(0, -90, 0));
		}
		AddMovementInput(Velocity);//���������ƶ��ٶȺͷ���
	}
}

void APacmanPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GameModeRef->CurrentState == EGameState::EPlay)
	{
		//�жϺ�character���ｺ���巢����ײ���ǲ����Ƕ������ʵ��,����ǣ���ɾ����ʵ��
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

