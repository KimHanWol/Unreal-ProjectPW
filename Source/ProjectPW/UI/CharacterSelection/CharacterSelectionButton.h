// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "CharacterSelectionButton.generated.h"

UCLASS()
class PROJECTPW_API UCharacterSelectionButton : public UPWUserWidget
{
	GENERATED_BODY()

public:

	void InitializeSelectionWidget(int32 CharacterNum, const struct FPWCharacterDataTableRow* CharacterData);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Num;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Desc;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Portrait;
};
