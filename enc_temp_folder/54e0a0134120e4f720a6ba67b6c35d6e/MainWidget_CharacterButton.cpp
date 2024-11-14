// LINK

//Default
#include "MainWidget_CharacterButton.h"

//Engine

//Game

void UMainWidget_CharacterButton::InitializeWidget(int32 Number, const FName& InCharacterKey)
{
	ButtonNum = Number;
	CharacterKey = InCharacterKey;

	InvalidateWidget();
}

void UMainWidget_CharacterButton::InvalidateWidget()
{
	Super::InvalidateWidget();

	if (IsValid(Text_Num) == true)
	{
		Text_Num->SetText(FText::FromString(FString::FromInt(ButtonNum)));
	}

	//Image_Portrait
	//PBar_Health
	//HBox_Stemina;
}
