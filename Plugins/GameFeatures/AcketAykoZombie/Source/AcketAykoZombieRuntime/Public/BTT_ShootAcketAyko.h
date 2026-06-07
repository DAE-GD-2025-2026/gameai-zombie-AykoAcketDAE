// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Zombies/BaseZombie.h"
#include "BTT_ShootAcketAyko.generated.h"

/**
 * 
 */
UCLASS()
class ACKETAYKOZOMBIERUNTIME_API UBTT_ShootAcketAyko : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTT_ShootAcketAyko();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	void FirePistol(ABaseZombie* zombie,APawn* player);
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector EnemyLocationKey;
};
