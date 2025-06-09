// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "MainMenuPopUp.generated.h"

UCLASS()
class PROJECTPW_API UMainMenuPopUp : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:

	void OnShowNotiWidget(const FText& NotificationText);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Noti;

};
