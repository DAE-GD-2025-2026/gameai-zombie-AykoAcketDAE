// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Items/ItemType.h" 
#include "BTT_UseItemAcketAyko.generated.h"

/**
 * 
 */
UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_UseItemAcketAyko : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_UseItemAcketAyko();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere)
	EItemType Item { EItemType::Garbage };
};
