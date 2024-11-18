// LINK

//Default
#include "CharacterInGameHUD.h"

//Engine
#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Component/PWCharacterHUDComponent.h"
#include "Core/PWEventManager.h"
#include "Helper/PWGameplayStatics.h"

UCharacterInGameHUD::UCharacterInGameHUD()
{
}

void UCharacterInGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == false)
	{
		ensure(false);
		return;
	}

	EventManager->HealthChangedDelegate.AddUObject(this, &UCharacterInGameHUD::OnHealthChanged);
}

void UCharacterInGameHUD::InitializeHUDWidget(class APWPlayerCharacter* InHUDOwnerCharacter)
{
	HUDOwnerCharacter = InHUDOwnerCharacter;

	if (IsValid(HUDOwnerCharacter) == true)
	{
		CharacterHUDComponent = Cast<UPWCharacterHUDComponent>(HUDOwnerCharacter->GetComponentByClass(UPWCharacterHUDComponent::StaticClass()));
	}
}

void UCharacterInGameHUD::OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth)
{
	if (IsValid(HUDOwnerCharacter) == false || HUDOwnerCharacter != TargetActor)
	{
		return;
	}

	if (IsValid(ProgressBar_Health) == true)
	{
		ProgressBar_Health->SetPercent(CurrentHealth / MaxHealth);
	}
}
