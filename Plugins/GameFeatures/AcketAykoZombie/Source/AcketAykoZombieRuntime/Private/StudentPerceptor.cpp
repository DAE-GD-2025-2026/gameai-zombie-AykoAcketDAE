// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor.h"
#include "AIController.h"
#include <limits>
#include "BehaviorTree/BlackboardComponent.h"
#include "Items/ItemType.h"

namespace Keys
{
	const FName EnemyKey(TEXT("Enemy"));
	const FName EnemyLocationKey(TEXT("EnemyLocation"));
	const FName HouseKey(TEXT("House"));
	const FName ItemKey(TEXT("Item"));
	const FName MoveToLocationKey(TEXT("MoveToLocation"));
	const FName HasWeaponKey(TEXT("HasWeapon"));
	const FName FeedKey(TEXT("Feed"));
	const FName HealKey(TEXT("Heal"));
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
	UpdateInventory();
	
	if (Actor->IsA(ABaseZombie::StaticClass()))
	{
		KnownZombies.Add(Cast<ABaseZombie>(Actor));
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
	UpdateInventory();
	return false;
}

bool UStudentPerceptor::UseItem(EItemType itemType)
{
	if (!Inventory) return false;
	if (HasItem[static_cast<int>(itemType)].first)
	{
		Inventory->UseItem(static_cast<int>(itemType));
		UpdateInventory();
		return true;
	}
	return false;
}

void UStudentPerceptor::UpdateInventory()
{
	if (!Inventory) return;
	for (int index{};index < Inventory->GetInventory().Num() - 1;++index)
	{
		if (!Inventory->GetInventory()[index]) continue;
		HasItem[index].first =  Inventory->GetInventory()[index]->GetValue() != 0;
		HasItem[index].second =  Inventory->GetInventory()[index]->GetValue();
	}
	if (HasItem[2].first || HasItem[3].first)
	{
		HasWeapon = true;
		Blackboard->SetValueAsBool(Keys::HasWeaponKey, HasWeapon);
	}
	else
	{
		HasWeapon = false;
		Blackboard->SetValueAsBool(Keys::HasWeaponKey, HasWeapon);
	}
	
	Health = GetOwner()->FindComponentByClass<UHealthComponent>();
	if (!Health) return;
	if (Health->GetHealth() <= 5) Blackboard->SetValueAsBool(Keys::HealKey, true);
	else Blackboard->SetValueAsBool(Keys::HealKey, false);
	
	Stamina = GetOwner()->FindComponentByClass<UStaminaComponent>();
	if (!Stamina) return;
	if (Stamina->GetCurrentStamina() <= 5) Blackboard->SetValueAsBool(Keys::FeedKey, true);
	else Blackboard->SetValueAsBool(Keys::FeedKey, false);
}

bool UStudentPerceptor::AreKnownZombiesAlive()
{
	TArray<ABaseZombie*> aliveZombies;
	for (TObjectPtr<ABaseZombie> zombie: KnownZombies)
	{
		if (!IsValid(zombie) || !zombie || zombie->IsActorBeingDestroyed())
			continue;
		aliveZombies.Add(zombie.Get());
	}
	std::pair<ABaseZombie*,float> closestHouse{nullptr,std::numeric_limits<float>::max()};
	for (auto zombie : aliveZombies)
	{
		float newHouseDistance = FVector::DistSquared(GetOwner()->GetActorLocation(),zombie->GetActorLocation());
		if (newHouseDistance < closestHouse.second)
			closestHouse = {zombie,newHouseDistance};
	}
	if (aliveZombies.Num() == 0) return false;
	Blackboard->SetValueAsVector(Keys::EnemyLocationKey, closestHouse.first->GetActorLocation());
	Blackboard->SetValueAsObject(Keys::EnemyKey, closestHouse.first);
	return true;
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
	UpdateInventory();
	if (!IsValid(item) || item->IsActorBeingDestroyed() || item->IsHidden())
	{
		SeenItems.Remove(item);
		IgnoredItems.Remove(item);
		UpdateInventory();
		return false;
	}
	if (type == 4)
	{
		if (pickUp)
		{
			Inventory->GrabItem(type,item);
			Inventory->RemoveItem(type);
			SeenItems.Remove(item);
			UpdateInventory();
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
			UpdateInventory();
		}
		return true;
	}
	IgnoredItems.Add(item);
	UpdateInventory();
	return false;
}
