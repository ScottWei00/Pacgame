// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Pacdot.h"

// Sets default values ���캯��
APacmanPlayer::APacmanPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Velocity = FVector(0, 0, 0);//��ʼ���ٶȴ�С

	PacmanPlayerDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanPlayerDisplayMesh"));//���������Ƕ�����ʾ���

	PacmanPlayerDisplayMesh->SetupAttachment(GetCapsuleComponent());//�󶨵�character�Դ��Ľ�����ײ����

}

// Called when the game starts or when spawned ��Ϸ��ʼ������begin play����ֻ������һ��
void APacmanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//ע�⣺��ײ��ⷽ����߼�ֻ��д��BeginPlay������д�ڹ��캯�����Ȼ��������
	//������character�������������ײ��⺯�����а�
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
		SetActorRotation(FRotator(0,0,0));//����character�����λ�ã���ת
	}
	else if (value < 0) 
	{
		SetActorRotation(FRotator(0,180,0));//ע��Frotator��Ĳ�����(Pitch, Yaw, Row)��Ӧ(y, z, x)
	}
	AddMovementInput(Velocity);
}

void APacmanPlayer::MoveY(float value)
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

void APacmanPlayer::OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�жϺ�character���ｺ���巢����ײ���ǲ����Ƕ������ʵ��,����ǣ���ɾ����ʵ��
	if (OtherActor->IsA(APacdot::StaticClass()))
	{
		OtherActor->Destroy();
	}

}

