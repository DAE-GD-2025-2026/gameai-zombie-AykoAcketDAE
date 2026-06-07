// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Shoot.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"

#include "Kismet/KismetMathLibrary.h"

UBTT_Shoot::UBTT_Shoot()
{
	NodeName = "Shoot Enemy";
	EnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Shoot, EnemyKey),ABaseZombie::StaticClass());
	EnemyKey.SelectedKeyName = TEXT("Enemy");
	EnemyLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Shoot, EnemyLocationKey));
	EnemyLocationKey.SelectedKeyName = TEXT("EnemyLocation");
}

EBTNodeResult::Type UBTT_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;
	
	APawn* ControlledPawn = AIController->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ControlledPawn  || !Blackboard)
		return EBTNodeResult::Failed;

	UStudentPerceptor* perceptor = ControlledPawn->GetComponentByClass<UStudentPerceptor>();
	if (!perceptor)
		return EBTNodeResult::Failed;
	
	ABaseZombie* enemy = Cast<ABaseZombie>(Blackboard->GetValueAsObject(EnemyKey.SelectedKeyName));
	if (!enemy)
		return EBTNodeResult::Failed;
	

	switch (perceptor->GetWeaponType())
	{
	case 2:
		FirePistol(enemy,ControlledPawn);
		if (perceptor->UseItem(static_cast<EItemType>(2)))
			return EBTNodeResult::Succeeded;
		break;
	case 3:
		FirePistol(enemy,ControlledPawn);
		if (perceptor->UseItem(static_cast<EItemType>(3)))
			return EBTNodeResult::Succeeded;
		break;
	default:
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}

void UBTT_Shoot::FirePistol(ABaseZombie* zombie,APawn* player)
{
	FRotator faceEnemyRotation = UKismetMathLibrary::FindLookAtRotation(player->GetActorLocation(),zombie->GetActorLocation());
	player->SetActorRotation(faceEnemyRotation);
	
}
