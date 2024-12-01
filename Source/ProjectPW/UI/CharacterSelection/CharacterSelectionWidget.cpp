// LINK

//Default
#include "CharacterSelectionWidget.h"

//Engine
#include "Components/Spacer.h"

//Game
#include "CharacterSelectionButton.h"
#include "Components/VerticalBox.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataAsset/PWGameData.h"

void UCharacterSelectionWidget::InitializeCharacterSelectionWidget()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(VBox_CharacterClass) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	for (int32 i = 0; i < CharacterDataList.Num(); i++)
	{
		const FPWCharacterDataTableRow* CharacterData = CharacterDataList[i];
		UCharacterSelectionButton* NewSelectionWidget = CreateWidget<UCharacterSelectionButton>(this, CharacterSelectionButtonClass);
		if (IsValid(NewSelectionWidget) == true)
		{
			NewSelectionWidget->InitializeSelectionWidget(i + 1, CharacterData);
			VBox_CharacterClass->AddChildToVerticalBox(NewSelectionWidget);
		}

		USpacer* NewSpacer = NewObject<USpacer>(this);
		if (IsValid(NewSpacer) == true)
		{
			NewSpacer->SetSize(FVector2D(0.f, CharacterSelectionButtonSpaceOffset));
			VBox_CharacterClass->AddChildToVerticalBox(NewSpacer);
		}
	}
}
