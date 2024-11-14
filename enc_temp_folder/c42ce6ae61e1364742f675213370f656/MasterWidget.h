// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "UI/PWUserWidget.h"

//Game

//Default
#include "MasterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UMasterWidget : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

private:

	void CloseAllWidget();

	void EnableMainWidget(bool bEnabled);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget* MainWidget;

private:

	UPROPERTY(Transient)
	TArray<class UPWUserWidget*> WidgetList;

};
