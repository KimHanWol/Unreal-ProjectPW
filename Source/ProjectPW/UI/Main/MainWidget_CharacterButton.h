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

	void InitializeWidget(int32 Number, const FName& InCharacterKey);

protected:

	virtual void InvalidateWidget() override;


protected:

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
	int32 ButtonNum = 0;

	UPROPERTY(Transient)
	FName CharacterKey = NAME_None;
};
