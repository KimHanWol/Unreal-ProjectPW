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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:
	
	void OnGameOver(class APWPlayerController* PlayerController, bool bLose);
	void OnTeamCharacterMoved(float CurrentTurnActivePoint);
	void OnTeamCharacterAllSpawned(const class APWPlayerController* TargetPlayerController, const TArray<TWeakObjectPtr<class APWPlayerCharacter>>& TargetCharacterList);
	void OnTargetIsOnCrosshair(class APWPlayerCharacter* AimingCharcter, bool bIsOnCrosshair);
	void OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander);
	void OnTurnChanged();
	void OnCharacterTriedToUseSkill(float TurnActivePointCost, bool bIsSuccessed);

	UFUNCTION()
	void OnReturnToMainMenuButtonPressed();

	void TryInitializeCharacterData();

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Turn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_TeamSide;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Result;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Result;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_ReturnMainMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Crosshair;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_TurnPoint_Commander;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_TurnPoint_Character;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_TurnPoint_Character_Back;

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_TurnPoint_Character;

	UPROPERTY(Transient)
	TArray<class UMainWidget_CharacterButton*> CharacterButtonList;

	UPROPERTY(Transient)
	FVector TurnPointProgressBaDefaultPos;

	FTimerHandle WaitCharacterDataInitTimerHandle;

	FTimerHandle TurnPointProgressBarShakeTimerHandle;

	UPROPERTY(Transient)
	float TurnActivePointDecreaseStartPercent;
};
