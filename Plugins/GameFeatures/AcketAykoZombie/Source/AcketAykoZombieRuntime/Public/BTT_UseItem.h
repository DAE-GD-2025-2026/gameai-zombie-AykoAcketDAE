// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Items/ItemType.h" 
#include "BTT_UseItem.generated.h"

/**
 * 
 */
UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_UseItem : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_UseItem();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere)
	EItemType Item { EItemType::Garbage };
};
