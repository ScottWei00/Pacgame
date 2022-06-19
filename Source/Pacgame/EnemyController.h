// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 为了实现AI控制糖豆人敌人的方法，首先定义一个AIcontroller的C++类，然后在场景中需要加入NavMeshBoundsVolume
 * 然后在C++中，进行三个函数，
 * 一个是重写自带的OnPossess(),来声明AI控制的是哪一个物体
 * 一个是自定义的OnMoveCompleted(),用来处理AI控制物体移动后的下一步
 * 最后一个是GotoNewDestination(),用来处理AI控制物体要去移动的位置
 */
UCLASS()
class PACGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	/// <summary>
	/// AI 通过OnPossess函数指定要控制的目标物体
	/// </summary>
	void OnPossess(class APawn* PawnToPossess) override;


	/// <summary>
	/// AI人机移动完成后
	/// </summary>
	/// <param name="RequestID"></param>
	/// <param name="Result"></param>
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


	/// <summary>
	/// AI人机移动到下一个新的目的地
	/// </summary>
	void GotoNewDestination();

private:
	class APacmanEnemy* ControlledEnemy;//定义一个糖豆人的变量，用来表示被AI控制的敌人
	
};
