// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ZombieCheck.generated.h"

/**
 * 
 */
UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTS_ZombieCheck : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_ZombieCheck();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	
};
