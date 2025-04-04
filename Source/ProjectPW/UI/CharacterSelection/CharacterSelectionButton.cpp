// LINK

//Default
#include "CharacterSelectionButton.h"

//Engine
#include "Components/Image.h"
#include "Components/TextBlock.h"

//Game
#include "Data/DataTable/PWCharacterDataTableRow.h"


void UCharacterSelectionButton::InitializeSelectionWidget(int32 InCharacterNum, const FPWCharacterDataTableRow* CharacterData)
{
	CharacterNum = InCharacterNum;

	if (ensure(IsValid(Text_Num) == true))
	{
		Text_Num->SetText(FText::FromString(FString::FromInt(CharacterNum)));
	}

	if (ensure(IsValid(Text_Name) == true))
	{
		Text_Name->SetText(FText::FromName(CharacterData->DisplayName));
	}

	if (ensure(IsValid(Text_Desc) == true))
	{
		Text_Desc->SetText(CharacterData->Description);
	}

	if (ensure(IsValid(Image_Icon) == true))
	{
		Image_Icon->SetBrushFromSoftTexture(CharacterData->Icon);
	}
}

FReply UCharacterSelectionButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	WidgetClickedDelegate.Broadcast(CharacterNum);

	return Reply;
}
