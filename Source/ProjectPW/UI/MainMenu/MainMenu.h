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

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:

	UFUNCTION()
	void OnCreateButtonPressed();

	UFUNCTION()
	void OnSearchButtonPressed();

	UFUNCTION()
	void OnQuitButtonPressed();

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(bool bWasSuccessful);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_CreateServer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_SearchServer;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UButton* Btn_Quit;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Process;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UOverlay* Overlay_Btn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UUserWidget* MainMenuPopUp;

	UPROPERTY(EditDefaultsOnly)
	FText SessionCreatingText;

	UPROPERTY(EditDefaultsOnly)
	FText SessionSearchingText;

	UPROPERTY(EditDefaultsOnly)
	FText SessionJoiningText;
};
