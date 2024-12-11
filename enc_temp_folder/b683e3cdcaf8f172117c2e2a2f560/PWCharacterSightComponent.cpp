// LINK

//Default
#include "PWCharacterSightComponent.h"

//Engine
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SpotLight.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"

void UPWCharacterSightComponent::BeginPlay()
{
	Super::BeginPlay();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &UPWCharacterSightComponent::OnCharacterPossessed);
	}
}

void UPWCharacterSightComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
		
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
	}
}

void UPWCharacterSightComponent::EnableSpotLight(bool bEnabled)
{
	bSpotLightEnabled = bEnabled;

	if (bSpotLightEnabled == true)
	{
		TrySpawnSpotLight();
	}

	InvalidateSight();
}

void UPWCharacterSightComponent::TrySpawnSpotLight()
{
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(CharacterSightSpotLight) == false)
	{
		CharacterSightSpotLight = GetWorld()->SpawnActor<ASpotLight>(CharacterSightSpotLightClass);
		if (IsValid(CharacterSightSpotLight) == true)
		{
			UCameraComponent* CameraComponent = PWPlayerCharacter->GetComponentByClass<UCameraComponent>();
			if (IsValid(CameraComponent) == true)
			{
				CharacterSightSpotLight->AttachToComponent(CameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
		}
	}

	if (IsValid(CharacterSpotLight) == false)
	{
		CharacterSpotLight = GetWorld()->SpawnActor<ASpotLight>(CharacterSpotLightClass);
		if (IsValid(CharacterSpotLight) == true)
		{
			CharacterSpotLight->AttachToComponent(PWPlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			CharacterSpotLight->SetActorLocation(PWPlayerCharacter->GetActorLocation() + FVector(0.f, 0.f, CharacterSpotLightHeightOffset));
		}
	}
}

void UPWCharacterSightComponent::OnCharacterPossessed(APawn* PossessedPawn, bool bIsCommander)
{
	bCharacterPossessed = PossessedPawn == GetOwner();
	InvalidateSight();
}

void UPWCharacterSightComponent::InvalidateSight()
{
	//TrySpawnSpotLight();

	if (IsValid(CharacterSightSpotLight) == false ||
		IsValid(CharacterSpotLight) == false)
	{
		ensure(false);
		return;
	}

	if (bSpotLightEnabled == true)
	{
		if (bCharacterPossessed == true)
		{
			CharacterSightSpotLight->SetActorHiddenInGame(false);
			CharacterSpotLight->SetActorHiddenInGame(false);
		}
		else
		{
			CharacterSightSpotLight->SetActorHiddenInGame(true);
			CharacterSpotLight->SetActorHiddenInGame(false);
		}
	}
	else
	{
		CharacterSightSpotLight->SetActorHiddenInGame(true);
		CharacterSpotLight->SetActorHiddenInGame(true);
	}
}
