// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PickUpItem.h"
#include "Items/BaseItem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"
#include "Items/BaseItem.h"

UBTT_PickUpItem::UBTT_PickUpItem()
{
	NodeName = "pick up item";
	ItemKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_PickUpItem, ItemKey),ABaseItem::StaticClass());
	ItemKey.SelectedKeyName = TEXT("Item");
}

EBTNodeResult::Type UBTT_PickUpItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	ABaseItem* value = Cast<ABaseItem>(Blackboard->GetValueAsObject(ItemKey.SelectedKeyName));
	if (!value)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("TargetItem null"));
		return EBTNodeResult::Failed;
	}
	if (perceptor->AddItemToInventory(value))
		return EBTNodeResult::Succeeded;
	return EBTNodeResult::Failed;
}
