// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "MainWidget.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API UMainWidget : public UPWUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void InvalidateWidget() override;

protected:

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:
	
	void OnGameOver(class APWPlayerController* PlayerController, bool bLose);
	void OnTeamCharacterMoved(float CurrentTurnActivePoint);
	void OnTeamCharacterAllSpawned(const class APWPlayerController* TargetPlayerController, const TArray<class APWPlayerCharacter*>& TeamCharcterList);
	void OnTargetIsOnCrosshair(class APWPlayerCharacter* AimingCharcter, bool bIsOnCrosshair);
	void OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander);
	void OnTurnChanged();

	UFUNCTION()
	void OnReturnToMainMenuButtonPressed();

	void TryInitializeCharacterData();

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Turn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Result;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Result;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_ReturnMainMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Crosshair;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_TurnPoint;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Commander;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Character;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget_CharacterButton* MainWidget_CharacterButton_1;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget_CharacterButton* MainWidget_CharacterButton_2;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget_CharacterButton* MainWidget_CharacterButton_3;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget_CharacterButton* MainWidget_CharacterButton_4;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget_CharacterButton* MainWidget_CharacterButton_5;

	UPROPERTY(Transient)
	TArray<class UMainWidget_CharacterButton*> CharacterButtonList;

	FTimerHandle WaitCharacterDataInitTimerHandle;
};
