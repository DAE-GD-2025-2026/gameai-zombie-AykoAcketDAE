// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Zombies/BaseZombie.h"
#include "Village/House/House.h"
#include "Items/BaseItem.h"
#include "Common/InventoryComponent.h"
#include "Common/HealthComponent.h"
#include "Common/StaminaComponent.h"
#include "Items/ItemType.h" 
#include "StudentPerceptor.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACKETAYKOZOMBIERUNTIME_API UStudentPerceptor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor();
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	UFUNCTION(BlueprintCallable)
	void MarkHouseVisited(AHouse* house);
	UFUNCTION(BlueprintCallable)
	bool AddItemToInventory(ABaseItem* item);
	UFUNCTION(BlueprintCallable)
	bool UseItem(EItemType itemType);
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();
private:
	
	UBlackboardComponent* GetBlackboardComp() const;
	void CheckHouseLocation();
	void CheckItemsAtLocation();
	bool IsItemNeeded(int type,ABaseItem* item,bool pickUp);
	
	TArray<TObjectPtr<AHouse>> VisitedHouses;
	TArray<TObjectPtr<AHouse>> SeenHouses;
	
	TArray<TObjectPtr<ABaseItem>> SeenItems;
	TArray<TObjectPtr<ABaseItem>> IgnoredItems;
	UBlackboardComponent* Blackboard{};

	UHealthComponent* Health{};
	UStaminaComponent* Stamina{};
	UInventoryComponent* Inventory{};
	//Items
	TArray<std::pair<bool,int>> HasItem{
		{false,0},
		{false,0},
		{false,0},
		{false,0}
		};
	bool HasWeapon{false};
	
};
