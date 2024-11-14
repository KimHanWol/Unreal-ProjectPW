// LINK

#pragma once

//Engine
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"

//Game

//Default
#include "PWUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UPWUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void BindEvents() {};
	virtual void UnbindEvents() {};

public:

	virtual void InvalidateWidget() {};

};
