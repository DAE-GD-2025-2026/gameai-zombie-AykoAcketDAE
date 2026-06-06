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
private:
	
	UBlackboardComponent* GetBlackboardComp() const;
	void CheckHouseLocation();
	
	TArray<TObjectPtr<AHouse>> VisitedHouses;
	TArray<TObjectPtr<AHouse>> SeenHouses;
	UBlackboardComponent* Blackboard;
};
