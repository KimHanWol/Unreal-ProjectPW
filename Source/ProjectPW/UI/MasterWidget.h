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

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget* MainWidget;

};
