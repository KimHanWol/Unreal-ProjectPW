// LINK

//Default
#include "GA_BuildWall_Preview.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "Abilities/GameplayAbility.h"

void UGA_BuildWall_Preview::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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

	GetAbilitySystemComponentFromActorInfo()->PrimaryComponentTick.bCanEverTick = true;

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
		StaticMeshComponent->SetVisibility(true);
	}
	else
	{
		StaticMeshComponent->SetVisibility(false);
	}
}