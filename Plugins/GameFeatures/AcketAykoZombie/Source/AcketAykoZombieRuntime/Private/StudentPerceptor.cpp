// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"
#include "AIController.h"
#include <limits>
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/ItemType.h"

namespace Keys
{
	const FName EnemyKey(TEXT("Enemy"));
	const FName HouseKey(TEXT("House"));
	const FName ItemKey(TEXT("Item"));
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
	if (ABaseItem* item = Cast<ABaseItem>(Actor))
	{
		if (IsItemNeeded(static_cast<int>(item->GetItemType()),item,false))
		{
			Inventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
			if (!Inventory) return;
			SeenItems.Add(item);
			CheckItemsAtLocation();
		}
	}
	AHouse* house = Cast<AHouse>(Actor);
	if (!house) return;
	
	if (Stimulus.WasSuccessfullySensed())
		SeenHouses.AddUnique(house);
	
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

bool UStudentPerceptor::AddItemToInventory(ABaseItem* item)
{
	Inventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (!Inventory) return false;
	SeenItems.Add(item);

	if (IsItemNeeded(static_cast<int>(item->GetItemType()),item,true))
		return true;
	CheckItemsAtLocation();
	return false;
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

void UStudentPerceptor::CheckItemsAtLocation()
{
	if (SeenItems.Num() == 0) return;
	ABaseItem* top = SeenItems.Top();
	if (!IsValid(top) || top->IsActorBeingDestroyed() || top->IsHidden())
	{
		SeenItems.Remove(top);
		IgnoredItems.Remove(top);
		SeenItems.Pop();
		return;
	}
	Blackboard->SetValueAsVector(Keys::MoveToLocationKey, top->GetActorLocation());
	Blackboard->SetValueAsObject(Keys::ItemKey, top);
}

bool UStudentPerceptor::IsItemNeeded(int type, ABaseItem* item, bool pickUp)
{
	if (!IsValid(item) || item->IsActorBeingDestroyed() || item->IsHidden())
	{
		SeenItems.Remove(item);
		IgnoredItems.Remove(item);
		return false;
	}
	if (type == 4)
	{
		if (pickUp)
		{
			Inventory->GrabItem(type,item);
			Inventory->RemoveItem(type);
			SeenItems.Remove(item);
		}
		return true;
	}
	if (!HasItem[type].first || item->GetValue() > HasItem[type].second)
	{
		if (pickUp)
		{
			Inventory->RemoveItem(type);
			Inventory->GrabItem(type,item);
			SeenItems.Remove(item);
			HasItem[type] = {true,item->GetValue()};
		}
		return true;
	}
	IgnoredItems.Add(item);
	return false;
}
