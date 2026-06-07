// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_WanderAcketAyko.generated.h"


UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_WanderAcketAyko : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_WanderAcketAyko();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MoveToLocationKey;
	UPROPERTY(EditAnywhere)
	float Radius = 1500.0f;
};
