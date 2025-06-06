﻿// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

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

	UFUNCTION(BlueprintCallable)
	void BP_PlayHoverSound();

	UFUNCTION(BlueprintCallable)
	void BP_PlayClickSound();

public:

	virtual void InvalidateWidget() {};

};
