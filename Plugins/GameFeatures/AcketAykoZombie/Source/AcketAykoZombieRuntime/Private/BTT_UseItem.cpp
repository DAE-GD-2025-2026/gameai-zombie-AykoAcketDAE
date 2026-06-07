// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UseItem.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"


UBTT_UseItem::UBTT_UseItem()
{
	NodeName = "Use Item";
}

EBTNodeResult::Type UBTT_UseItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	if (Item == EItemType::Garbage) return EBTNodeResult::Failed;
	if (perceptor->UseItem(Item))
		return EBTNodeResult::Succeeded;
	
	return EBTNodeResult::Failed;
}
