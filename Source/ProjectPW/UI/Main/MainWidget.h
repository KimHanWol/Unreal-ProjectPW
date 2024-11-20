// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UMainWidget : public UPWUserWidget
{
	GENERATED_BODY()

public:

	virtual void InvalidateWidget() override;

protected:

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:
	
	void OnTurnChanged(bool bIsMyTurn);
	void OnGameOver(bool bWon);
	void OnTeamCharacterMoved(float CurrentTurnActivePoint);
	void OnTargetIsonCrosshair(bool bIsOnCrosshair);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* Text_Turn;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* Text_Result;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* Text_Crosshair;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_TurnPoint;
};
