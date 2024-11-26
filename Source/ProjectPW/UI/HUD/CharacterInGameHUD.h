// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "CharacterInGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UCharacterInGameHUD : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:

	void InitializeHUDWidget(class APWPlayerCharacter* InHUDOwnerCharacter);
	void OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_Health;

private:

	UPROPERTY(Transient)
	class APWPlayerCharacter* HUDOwnerCharacter;
};
