// Fill out your copyright notice in the Description page of Project Settings.
#include "BTT_Wander.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"


UBTT_Wander::UBTT_Wander()
{
	NodeName = "Wander";
	MoveToLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTT_Wander, MoveToLocationKey));
}

EBTNodeResult::Type UBTT_Wander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;
	
	APawn* ControlledPawn = AIController->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ControlledPawn  || !Blackboard)
		return EBTNodeResult::Failed;
	
	FVector RandomDir = FMath::VRand();
	RandomDir.Z = 0.0f;
	RandomDir.Normalize();

	const FVector RandomLoc = RandomDir * FMath::FRandRange(0.0f, Radius) + ControlledPawn->GetActorLocation() ;
	Blackboard->SetValueAsVector(MoveToLocationKey.SelectedKeyName, RandomLoc);
	
	return EBTNodeResult::Succeeded;
}

