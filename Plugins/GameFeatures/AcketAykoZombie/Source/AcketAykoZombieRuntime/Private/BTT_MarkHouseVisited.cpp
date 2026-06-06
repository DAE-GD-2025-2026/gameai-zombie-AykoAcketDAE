// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MarkHouseVisited.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"
#include "Village/House/House.h"

UBTT_MarkHouseVisited::UBTT_MarkHouseVisited()
{
	NodeName = "Mark House as Visited";
	HouseKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MarkHouseVisited, HouseKey),AHouse::StaticClass());
	HouseKey.SelectedKeyName = TEXT("House");
}

EBTNodeResult::Type UBTT_MarkHouseVisited::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	AHouse* value = Cast<AHouse>(Blackboard->GetValueAsObject(HouseKey.SelectedKeyName));
	if (!value)
		return EBTNodeResult::Succeeded;
	
	perceptor->MarkHouseVisited(value);
	return EBTNodeResult::Succeeded;
}
