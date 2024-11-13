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

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UTextBlock* Text_Turn;
};
