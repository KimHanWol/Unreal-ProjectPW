// LINK

//Default
#include "PWGameplayAbilityBase.h"

//Engine

//Game
#include "AbilitySystem/Task/PWAbilityTask_Tick.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWPlayerState.h"

void UPWGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UPWAbilityTask_Tick* AbilityTask_Tick = UPWAbilityTask_Tick::MakeTickTask(this, -1.f);
	AbilityTask_Tick->OnTick.BindUObject(this, &UPWGameplayAbilityBase::OnTick);
	AbilityTask_Tick->ReadyForActivation();
}

bool UPWGameplayAbilityBase::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

	return bResult;
}

bool UPWGameplayAbilityBase::EquipmentActorHitTest(float InMaxRange, ECollisionChannel TargetChannel, FHitResult& OutHitResult, FRotator& OutViewPointRotation)
{
	if (GetCurrentActorInfo() == nullptr)
	{
		ensure(false);
		return false;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetCurrentActorInfo()->AvatarActor);
	if (IsValid(OwnerCharacter) == false)
	{
		return false;
	}
		
	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		return false;
	}

	bool bHitSuccess = false;
	FVector ViewPointLocation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, OutViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + OutViewPointRotation.Vector() * InMaxRange;

	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerCharacter);

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(OutHitResult, ViewPointLocation, EndLocation, TargetChannel, Params);
	}

	return bHitSuccess;
}