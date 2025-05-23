﻿// LINK

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

	void InitializeSelectionWidget(int32 InCharacterNum, const struct FPWCharacterDataTableRow* CharacterData);

protected:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWidgetClicked, int32 CharacterNum);
	FOnWidgetClicked WidgetClickedDelegate;

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Num;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Desc;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Icon;

	UPROPERTY(Transient)
	int32 CharacterNum;
};
