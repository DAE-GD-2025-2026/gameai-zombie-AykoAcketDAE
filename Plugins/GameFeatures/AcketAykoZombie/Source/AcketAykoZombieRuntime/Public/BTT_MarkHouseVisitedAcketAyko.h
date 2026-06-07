// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MarkHouseVisitedAcketAyko.generated.h"

UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_MarkHouseVisitedAcketAyko : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_MarkHouseVisitedAcketAyko();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HouseKey;
};
