// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PickUpItemAcketAyko.h"
#include "Items/BaseItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptorAcketAyko.h"


UBTT_PickUpItemAcketAyko::UBTT_PickUpItemAcketAyko()
{
	NodeName = "pick up item";
	ItemKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_PickUpItemAcketAyko, ItemKey),ABaseItem::StaticClass());
	ItemKey.SelectedKeyName = TEXT("Item");
}

EBTNodeResult::Type UBTT_PickUpItemAcketAyko::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;
	
	APawn* ControlledPawn = AIController->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ControlledPawn  || !Blackboard)
		return EBTNodeResult::Failed;

	UStudentPerceptorAcketAyko* perceptor = ControlledPawn->GetComponentByClass<UStudentPerceptorAcketAyko>();
	if (!perceptor)
		return EBTNodeResult::Failed;
	
	ABaseItem* value = Cast<ABaseItem>(Blackboard->GetValueAsObject(ItemKey.SelectedKeyName));
	if (!value)
		return EBTNodeResult::Failed;
	
	if (perceptor->AddItemToInventory(value))
		return EBTNodeResult::Succeeded;
	return EBTNodeResult::Failed;
}
