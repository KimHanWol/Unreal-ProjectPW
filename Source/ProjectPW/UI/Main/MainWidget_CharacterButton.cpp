// LINK

//Default
#include "MainWidget_CharacterButton.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWPlayerState.h"
#include "Helper/PWGameplayStatics.h"

void UMainWidget_CharacterButton::InitializeWidget(const FName& InCharacterKey)
{
	CharacterKey = InCharacterKey;

	InvalidateWidget();
}

FReply UMainWidget_CharacterButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//TODO: 키보드 입력과 합칠 수 있으면 좋을 듯 (예: 이벤트시스템)
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false)
	{
		ensure(false);
		return FReply::Unhandled();
	}

	const APWPlayerState* PWPlayerState = LocalPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return FReply::Unhandled();
	}

	if (PWPlayerState->IsMyTurn() == false)
	{
		return FReply::Unhandled();
	}

	LocalPlayerController->SelectCharacter(ButtonIndex + 1);

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UMainWidget_CharacterButton::InvalidateWidget()
{
	Super::InvalidateWidget();

	if (IsValid(Text_Num) == true)
	{
		Text_Num->SetText(FText::FromString(FString::FromInt(ButtonIndex + 1)));
	}

	//Image_Portrait
	//PBar_Health
	//HBox_Stemina;
}
