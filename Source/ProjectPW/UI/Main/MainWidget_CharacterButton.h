// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "MainWidget_CharacterButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UMainWidget_CharacterButton : public UPWUserWidget
{
	GENERATED_BODY()

public:

	void InitializeWidget(const FName& InCharacterKey);

	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

protected:

	virtual void InvalidateWidget() override;

public:

	DECLARE_MULTICAST_DELEGATE(FButtonPressed)
	FButtonPressed ButtonPressedDelegate;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 ButtonIndex = 0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Num;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Portrait;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* PBar_Health;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UHorizontalBox* HBox_Stemina;

private:

	UPROPERTY(Transient)
	FName CharacterKey = NAME_None;
};
