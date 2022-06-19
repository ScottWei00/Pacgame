// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "PacmanEnemy.h" 
#include "NavigationSystem.h"

void AEnemyController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);//super�������ø���ķ���

	ControlledEnemy = Cast<APacmanEnemy>(PawnToPossess);
}

void AEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	GotoNewDestination();
}

void AEnemyController::GotoNewDestination()
{
	UNavigationSystemV1* NavMesh = UNavigationSystemV1::GetCurrent(this);
	if (NavMesh)
	{
		const float SearchRadius = 10000.f;
		FNavLocation RandomLocation;
		//������GetActorLocationΪԭ�㣬SearchRadiusΪ�뾶�ķ�Χ�ڵ�һ��λ�ã�����RandomLocation
		const bool bNewDestinationFound = NavMesh->GetRandomReachablePointInRadius(
			ControlledEnemy->GetActorLocation(),SearchRadius,RandomLocation);
		if (bNewDestinationFound)//�ҵ���λ�ú��ƶ����õ�RandomLocation
		{
			MoveToLocation(RandomLocation, 50.f);
		}
	}
}
