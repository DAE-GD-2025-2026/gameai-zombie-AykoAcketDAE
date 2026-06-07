// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UseItemAcketAyko.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptorAcketAyko.h"


UBTT_UseItemAcketAyko::UBTT_UseItemAcketAyko()
{
	NodeName = "Use Item";
}

EBTNodeResult::Type UBTT_UseItemAcketAyko::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	if (Item == EItemType::Garbage) return EBTNodeResult::Failed;
	if (perceptor->UseItem(Item))
		return EBTNodeResult::Succeeded;
	
	return EBTNodeResult::Failed;
}
