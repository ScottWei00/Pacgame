// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h" 
#include "PacmanEnemy.h" 
#include "NavigationSystem.h"

void AEnemyController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);//super用来调用父类的方法

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
		//搜索以GetActorLocation为原点，SearchRadius为半径的范围内的一个位置，赋给RandomLocation
		const bool bNewDestinationFound = NavMesh->GetRandomReachablePointInRadius(
			ControlledEnemy->GetActorLocation(),SearchRadius,RandomLocation);
		if (bNewDestinationFound)//找到该位置后，移动到该点RandomLocation
		{
			MoveToLocation(RandomLocation, 50.f);//该语句完成后跳到函数OnMoveCompleted()
		}
	}
} 
