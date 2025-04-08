// LINK

#pragma once

//Engine
#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"

//Game

//Default
#include "PWGameplayAbilityBase.generated.h"


UCLASS()
class PROJECTPW_API UPWGameplayAbilityBase: public UGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void OnTick(class UPWAbilityTask_Tick* AbilityTask_Tick, float DeltaTime) {};

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool EquipmentActorHitTest(float InMaxRange, ECollisionChannel TargetChannel,  FHitResult& OutHitResult, FRotator& OutViewPointRotation);
	virtual bool IsInteractableActor(AActor* TargetActor) { return true; }

public:

	float GetTurnActivePointCost() const { return TurnActivePointCost; }
	float GetMaxRange() const { return MaxRange; }

public:

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName SkillDisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TSoftObjectPtr<class UTexture2D> SkillIcon;

	//해당 어빌리티를 쓰는 데 드는 비용
	UPROPERTY(EditDefaultsOnly, Category = "Spec")
	float TurnActivePointCost = 0.f;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Spec")
	float MaxRange = 1000.f;

};
