// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ZombieCheckAcketAyko.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "StudentPerceptorAcketAyko.h"

UBTS_ZombieCheckAcketAyko::UBTS_ZombieCheckAcketAyko()
{
	NodeName = "See if known zombies are still alive";
}

void UBTS_ZombieCheckAcketAyko::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return;
	
	APawn* ControlledPawn = AIController->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!ControlledPawn  || !Blackboard)
		return;

	UStudentPerceptorAcketAyko* perceptor = ControlledPawn->GetComponentByClass<UStudentPerceptorAcketAyko>();
	if (!perceptor)
		return;
	perceptor->AreKnownZombiesAlive();
}
