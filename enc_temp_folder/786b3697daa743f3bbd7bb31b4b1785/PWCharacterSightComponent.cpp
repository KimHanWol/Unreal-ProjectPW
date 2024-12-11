// LINK

//Default
#include "PWCharacterSightComponent.h"

//Engine
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "Engine/SpotLight.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"

void UPWCharacterSightComponent::BeginPlay()
{
	Super::BeginPlay();

	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == false)
	{
		ensure(false);
		return;
	}
}

void UPWCharacterSightComponent::EnableSpotLight(bool bEnabled)
{
	//if (IsValid(CharacterSightSpotLight) == false)
	//{
	//	ensure(false);
	//	return;
	//}

	//if (bEnabled == true)
	//{
	//	CharacterSightSpotLight->SetActorHiddenInGame(false);
	//}
	//else
	//{
	//	CharacterSightSpotLight->SetActorHiddenInGame(true);
	//}
}

ASpotLight* UPWCharacterSightComponent::SpawnCharacterSightSpotLight()
{
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == false)
	{
		ensure(false);
		return nullptr;
	}

	ASpotLight* CharacterSightSpotLight = GetWorld()->SpawnActor<ASpotLight>();
	if (IsValid(CharacterSightSpotLight) == true)
	{
		CharacterSightSpotLight->SetMobility(EComponentMobility::Movable);
		CharacterSightSpotLight->SetReplicates(false);

		UCameraComponent* CameraComponent = PWPlayerCharacter->GetComponentByClass<UCameraComponent>();
		if (IsValid(CameraComponent) == true)
		{
			CharacterSightSpotLight->AttachToComponent(CameraComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);		
		}

		CharacterSightSpotLight->GetRootComponent();

		USpotLightComponent* SpotLightComponent = Cast<USpotLightComponent>(CharacterSightSpotLight->GetLightComponent());
		if (IsValid(SpotLightComponent) == true)
		{
			SpotLightComponent->SetIntensity(SightSpotLightIntensity);
			SpotLightComponent->SetCastShadows(false);
			SpotLightComponent->SetAttenuationRadius(SightSpotLightAttenuationRadius);
		}
	}

	return CharacterSightSpotLight;
}

ASpotLight* UPWCharacterSightComponent::SpawnCharacterSpotLight()
{
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(PWPlayerCharacter) == false)
	{
		ensure(false);
		return nullptr;
	}

	ASpotLight* CharacterSpotLight = GetWorld()->SpawnActor<ASpotLight>();
	if (IsValid(CharacterSpotLight) == true)
	{
		CharacterSpotLight->SetMobility(EComponentMobility::Movable);
		CharacterSpotLight->AttachToComponent(PWPlayerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		CharacterSpotLight->SetActorLocation(PWPlayerCharacter->GetActorLocation() + FVector(0.f, 0.f, CharacterSpotLightHeightOffset));
		CharacterSpotLight->SetReplicates(false);

		USpotLightComponent* SpotLightComponent = Cast<USpotLightComponent>(CharacterSpotLight->GetLightComponent());
		if (IsValid(SpotLightComponent) == true)
		{
			SpotLightComponent->SetIntensity(CharacterSpotLightIntensity);
			SpotLightComponent->SetCastShadows(false);
			SpotLightComponent->SetInnerConeAngle(CharacterSpotLightInnerConeAngle);
			SpotLightComponent->SetOuterConeAngle(CharacterSpotLightOuterConeAngle);
		}
	}

	return CharacterSpotLight;
}
