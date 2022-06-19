// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * Ϊ��ʵ��AI�����Ƕ��˵��˵ķ��������ȶ���һ��AIcontroller��C++�࣬Ȼ���ڳ�������Ҫ����NavMeshBoundsVolume
 * Ȼ����C++�У���������������
 * һ������д�Դ���OnPossess(),������AI���Ƶ�����һ������
 * һ�����Զ����OnMoveCompleted(),��������AI���������ƶ������һ��
 * ���һ����GotoNewDestination(),��������AI��������Ҫȥ�ƶ���λ��
 */
UCLASS()
class PACGAME_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	/// <summary>
	/// AI ͨ��OnPossess����ָ��Ҫ���Ƶ�Ŀ������
	/// </summary>
	void OnPossess(class APawn* PawnToPossess) override;


	/// <summary>
	/// AI�˻��ƶ���ɺ�
	/// </summary>
	/// <param name="RequestID"></param>
	/// <param name="Result"></param>
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


	/// <summary>
	/// AI�˻��ƶ�����һ���µ�Ŀ�ĵ�
	/// </summary>
	void GotoNewDestination();

private:
	class APacmanEnemy* ControlledEnemy;//����һ���Ƕ��˵ı�����������ʾ��AI���Ƶĵ���
	
};
