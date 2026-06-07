// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateInventory.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"

UBTS_UpdateInventory::UBTS_UpdateInventory()
{
	NodeName = "Update Inventory";
	Interval = 0.5f;     
	
}

void UBTS_UpdateInventory::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return;
	
	APawn* ControlledPawn = AIController->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ControlledPawn  || !Blackboard)
		return;

	UStudentPerceptor* perceptor = ControlledPawn->GetComponentByClass<UStudentPerceptor>();
	if (!perceptor)
		return;
	perceptor->UpdateInventory();
}
