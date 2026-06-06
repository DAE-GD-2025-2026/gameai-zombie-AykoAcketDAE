// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"
#include "AIController.h"
#include <limits>
#include "BehaviorTree/BlackboardComponent.h"

namespace Keys
{
	const FName EnemyKey(TEXT("Enemy"));
	const FName HouseKey(TEXT("House"));
	const FName MoveToLocationKey(TEXT("MoveToLocation"));
	const FName HasWeaponKey(TEXT("HasWeapon"));
}

UStudentPerceptor::UStudentPerceptor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor::BeginPlay()
{
	Super::BeginPlay();

	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor::OnPerceptionUpdated);
	}
}

void UStudentPerceptor::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Green, Actor->GetName());

	Blackboard = GetBlackboardComp();
	if (!Blackboard) return;

	if (Actor->IsA(ABaseZombie::StaticClass()))
	{
		Blackboard->SetValueAsObject(Keys::EnemyKey, Actor);
		return;
	}

	AHouse* house = Cast<AHouse>(Actor);
	if (!house) return;

	// Only ever ADD to SeenHouses, never remove on perception lost
	if (Stimulus.WasSuccessfullySensed())
		SeenHouses.AddUnique(house);

	// Always recalculate closest from accumulated seen houses
	CheckHouseLocation();
}

void UStudentPerceptor::MarkHouseVisited(AHouse* house)
{
	if (!house)return;
	if (Blackboard->GetValueAsObject(Keys::HouseKey) == house)
	{
		VisitedHouses.Add(house);
		SeenHouses.Remove(house);
		Blackboard->ClearValue(Keys::HouseKey);
	}
	CheckHouseLocation();
	
}

UBlackboardComponent* UStudentPerceptor::GetBlackboardComp() const
{
	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner)
		return nullptr;
	AAIController* AIController = Cast<AAIController>(PawnOwner->GetController());
	if (!AIController)
		return nullptr;
	return AIController->GetBlackboardComponent();
}

void UStudentPerceptor::CheckHouseLocation()
{
	std::pair<AHouse*,float> closestHouse{nullptr,std::numeric_limits<float>::max()};
	for (auto house : SeenHouses)
	{
		if (VisitedHouses.Contains(house)) continue;
		float newHouseDistance = FVector::DistSquared(GetOwner()->GetActorLocation(),house->GetActorLocation());
		if (newHouseDistance < closestHouse.second)
			closestHouse = {house,newHouseDistance};
	}
	if (closestHouse.first)
	{
		Blackboard->SetValueAsVector(Keys::MoveToLocationKey, closestHouse.first->GetActorLocation());
		Blackboard->SetValueAsObject(Keys::HouseKey, closestHouse.first);
	}
	else
		Blackboard->ClearValue(Keys::HouseKey);
}
