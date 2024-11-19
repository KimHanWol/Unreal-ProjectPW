// LINK

//Default
#include "CharacterInGameHUD.h"

//Engine
#include "Camera/CameraComponent.h"
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"


void UCharacterInGameHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->HealthChangedDelegate.AddUObject(this, &UCharacterInGameHUD::OnHealthChanged);
	}
}

void UCharacterInGameHUD::InitializeHUDWidget(class APWPlayerCharacter* InHUDOwnerCharacter)
{
	HUDOwnerCharacter = InHUDOwnerCharacter;
}

void UCharacterInGameHUD::OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth)
{
	// TODO: 체력 반영 안되는 이유 찾아야 함
	// 값은 정상적으로 세팅되고 F8로 나가면 잘 보이는데 인게임에서만 아무런 변화가 없음
	// Visibility 같은 변화도 없음.
	if (IsValid(HUDOwnerCharacter) == false || HUDOwnerCharacter != TargetActor)
	{
		return;
	}

	if (IsValid(ProgressBar_Health) == true)
	{
		ProgressBar_Health->SetPercent(FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f));
	}
}


