// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ZombieCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptor.h"

UBTS_ZombieCheck::UBTS_ZombieCheck()
{
	NodeName = "See if known zombies are still alive";
}

void UBTS_ZombieCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	perceptor->AreKnownZombiesAlive();
}
