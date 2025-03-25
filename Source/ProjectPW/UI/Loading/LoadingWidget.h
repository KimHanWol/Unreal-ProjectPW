// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "LoadingWidget.generated.h"

UCLASS()
class PROJECTPW_API ULoadingWidget : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

public:

	virtual void InvalidateWidget() override;
	void InitializeLoadingWidget(FName LevelKey);

private:

	void OnAllPlayerReadyToStart();
	void OnLoadingFinished();

protected:

	UPROPERTY(Transient)
	float CurrentLoadingDuration = 0.f;

	UPROPERTY(EditAnywhere)
	float MinLoadingDuration = 3.f;

	UPROPERTY(EditAnywhere)
	float WaitingOtherPlayerReadyPoint = 0.7f;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Background;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_MapName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* ProgressBar_Loading;

	FTimerHandle LoadingTimerHandle;

	UPROPERTY(Transient)
	bool bIsAllPlayerReadyToStart = false;

};
