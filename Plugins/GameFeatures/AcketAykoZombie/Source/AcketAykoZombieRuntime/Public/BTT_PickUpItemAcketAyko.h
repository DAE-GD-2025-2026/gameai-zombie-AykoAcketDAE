// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_PickUpItemAcketAyko.generated.h"

/**
 * 
 */
UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_PickUpItemAcketAyko : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_PickUpItemAcketAyko();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ItemKey;
	
};
