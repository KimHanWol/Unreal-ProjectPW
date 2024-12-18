// LINK

//Default
#include "GA_BuildWall_Preview.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "Abilities/GameplayAbility.h"
#include "Actor/Character/PWPlayerCharacter.h"

void UGA_BuildWall_Preview::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo == nullptr)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* TargetPlayerCharacter = Cast<APWPlayerCharacter>(ActorInfo->AvatarActor);
	if (IsValid(TargetPlayerCharacter) == false)
	{
		ensure(false);
		return;
	}

	//only for local
	if (TargetPlayerCharacter->IsLocallyControlled() == false)
	{
		return;
	}

	if (BuildPreivewObjectClass == nullptr)
	{
		ensure(false);
		return;
	}

	BuildPreviewObject = GetWorld()->SpawnActor(BuildPreivewObjectClass);
	if (ensure(IsValid(BuildPreviewObject) == true))
	{
		BuildPreviewObject->SetActorEnableCollision(false);
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(BuildPreviewObject->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (IsValid(StaticMeshComponent) == true)
		{
			StaticMeshComponent->SetVisibility(false);
			if (IsValid(BuildPreviewMI) == true)
			{
				for (int32 i = 0; i < StaticMeshComponent->GetMaterials().Num(); i++)
				{
					StaticMeshComponent->SetMaterial(i, BuildPreviewMI);
				}
			}
		}
	}

	bBuildPreviewMode = true;
}

void UGA_BuildWall_Preview::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	bBuildPreviewMode = false;
	if (IsValid(BuildPreviewObject) == true)
	{
		BuildPreviewObject->Destroy();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_BuildWall_Preview::OnTick(UPWAbilityTask_Tick* AbilityTask_Tick, float DeltaTime)
{
	Super::OnTick(AbilityTask_Tick, DeltaTime);

	if (bBuildPreviewMode == false)
	{
		return;
	}

	if (IsValid(BuildPreviewObject) == false)
	{
		return;
	}

	if (GetCurrentActorInfo() == nullptr)
	{
		ensure(false);
		return;
	}

	TWeakObjectPtr<AActor> AvatarActor = GetCurrentActorInfo()->AvatarActor;
	if (AvatarActor.IsValid() == false)
	{
		ensure(false);
		return;
	}

	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(BuildPreviewObject->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (IsValid(StaticMeshComponent) == false)
	{
		return;
	}

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_WorldStatic, HitResult, ViewPointRotation);
	if (bHitSuccess == true)
	{
		BuildPreviewObject->SetActorLocation(HitResult.Location);
		FVector LookVector = AvatarActor->GetActorLocation() - HitResult.Location;
		BuildPreviewObject->SetActorRotation(LookVector.Rotation());

		StaticMeshComponent->SetVisibility(true);
	}
	else
	{
		StaticMeshComponent->SetVisibility(false);
	}
}