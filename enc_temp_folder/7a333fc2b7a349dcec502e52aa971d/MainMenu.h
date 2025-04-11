// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "MainMenu.generated.h"

UCLASS()
class PROJECTPW_API UMainMenu: public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized() override;
	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:

	UFUNCTION()
	void OnPreviousButtonPressed();

	UFUNCTION()
	void OnNextButtonPressed();

	UFUNCTION()
	void OnCreateButtonPressed();

	UFUNCTION()
	void OnSearchButtonPressed();

	UFUNCTION()
	void OnQuitButtonPressed();

	UFUNCTION()
	void OnStopMatchMakingButtonPressed();

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(bool bWasSuccessful);

	void UpdateCurrentLevelData(); // 애니메이션 중간에 바꿔야 해서 따로 분리
	void UpdateLevelData();

	int32 GetPrevLevelIndex();
	int32 GetNextLevelIndex();

	UFUNCTION()
	void OnLevelSlideAnimFinished();

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Main;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_BG_Prev;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_BG;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_BG_Next;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_LevelName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_Previous;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_Next;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_CreateSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_SearchSession;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_Quit;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Waiting;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Process;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_StopMatchMaking;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UUserWidget* MainMenuPopUp;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_LevelSlide_Prev;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_LevelSlide_Next;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_MatchMaking;

	UPROPERTY(EditDefaultsOnly)
	FText SessionCreatingText;

	UPROPERTY(EditDefaultsOnly)
	FText SessionSearchingText;

	UPROPERTY(EditDefaultsOnly)
	FText SessionJoiningText;

	UPROPERTY(Transient)
	bool bIsAnimationPlaying = false;

	UPROPERTY(Transient)
	int32 CurrentSelectedLevelIndex = 0;

	UPROPERTY(Transient)
	bool bIsMatchMaking = false;
};
