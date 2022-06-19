// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanEnemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PacmanPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
APacmanEnemy::APacmanEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PacmanEnemyDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanEnemyDisplayMesh"));//创建敌人组件
	PacmanEnemyDisplayMesh->SetupAttachment(GetCapsuleComponent());//绑定到character自带的胶囊体组件

	bIsDead = false;
	bIsVulnerable = false;

	DeadTime = 5.f;
	VulnerableTime = 5.f;
	

	NormalSpeed = 600.f;
	VulnerableSpeed = 100.f;
}



// Called when the game starts or when spawned
void APacmanEnemy::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanEnemy::OnEnemyBeginOverlay);

	SetMovement(true);
}

// Called every frame
void APacmanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacmanEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APacmanEnemy::SetVulnerable(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer)
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, Time, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, VulnerableTime, false);
	}
	if (bIsVulnerable)
	{
		return;
	}

	bIsVulnerable = true;
	SetMovement(true);
}

void APacmanEnemy::SetDead(bool bRestoreTimer, float Time)
{
	if (bRestoreTimer && bIsDead)
	{
		GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, Time, false);
		return;
	}
	if(bIsDead)
	{
		return;
	}

	bIsDead = true;
	SetMovement(true);
	SetActorLocation(StartLocation);
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, DeadTime, false);
}

void APacmanEnemy::SetNormal()
{
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);
	GetWorldTimerManager().ClearTimer(DeadTimerHandle);

	bIsVulnerable = false;
	bIsDead = false;
	SetMovement(true);
}

void APacmanEnemy::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		if (bIsDead == false && bIsVulnerable == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
		if (bIsDead == false && bIsVulnerable == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = VulnerableSpeed;
		}
		if (bIsDead == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

//enemy和player发生碰撞时，摧毁player
void APacmanEnemy::OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacmanPlayer::StaticClass()))
	{
		if (bIsDead)
		{
			return;
		}
		if (bIsVulnerable)
		{
			SetDead();
		}
		else
		{
			Cast<APacmanPlayer>(OtherActor)->Injured();
		}
	}
} 