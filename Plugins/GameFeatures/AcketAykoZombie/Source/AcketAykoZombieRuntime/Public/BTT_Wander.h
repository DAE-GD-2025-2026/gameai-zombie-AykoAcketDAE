// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Wander.generated.h"


UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_Wander : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_Wander();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MoveToLocationKey;
	UPROPERTY(EditAnywhere)
	float Radius = 1500.0f;
};
