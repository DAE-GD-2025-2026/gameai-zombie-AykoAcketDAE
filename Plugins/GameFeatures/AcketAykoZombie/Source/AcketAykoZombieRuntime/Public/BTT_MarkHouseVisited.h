// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MarkHouseVisited.generated.h"

UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_MarkHouseVisited : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_MarkHouseVisited();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HouseKey;
};
