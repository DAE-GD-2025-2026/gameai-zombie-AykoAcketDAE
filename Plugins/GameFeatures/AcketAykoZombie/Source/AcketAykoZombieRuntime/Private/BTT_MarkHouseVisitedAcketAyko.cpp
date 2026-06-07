// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_MarkHouseVisitedAcketAyko.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptorAcketAyko.h"
#include "Village/House/House.h"

UBTT_MarkHouseVisitedAcketAyko::UBTT_MarkHouseVisitedAcketAyko()
{
	NodeName = "Mark House as Visited";
	HouseKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_MarkHouseVisitedAcketAyko, HouseKey),AHouse::StaticClass());
	HouseKey.SelectedKeyName = TEXT("House");
}

EBTNodeResult::Type UBTT_MarkHouseVisitedAcketAyko::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	AHouse* value = Cast<AHouse>(Blackboard->GetValueAsObject(HouseKey.SelectedKeyName));
	if (!value)
		return EBTNodeResult::Succeeded;
	
	perceptor->MarkHouseVisited(value);
	return EBTNodeResult::Succeeded;
}
