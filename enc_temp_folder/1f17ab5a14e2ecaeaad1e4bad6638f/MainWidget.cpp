// LINK

//Default
#include "MainWidget.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Helper/PWGameplayStatics.h"

void UMainWidget::InvalidateWidget()
{
	Super::InvalidateWidget();

	const APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
	if (IsValid(PWPlayerState) == true)
	{
		OnTurnChanged(PWPlayerState->IsMyTurn());
	}
}

void UMainWidget::BindEvents()
{
	Super::BindEvents();

	APWPlayerController* LocalPlayerController = Cast<APWPlayerController>(UPWGameplayStatics::GetLocalPlayerController(this));
	if (IsValid(LocalPlayerController) == true)
	{
		LocalPlayerController->TurnChangedDelegate.AddUObject(this, &UMainWidget::OnTurnChanged);
		LocalPlayerController->PlayerStateLoadedDelegate.AddUObject(this, &UMainWidget::InvalidateWidget);
	}
}

void UMainWidget::UnbindEvents()
{
	Super::UnbindEvents();

	APWPlayerController* LocalPlayerController = Cast<APWPlayerController>(UPWGameplayStatics::GetLocalPlayerController(this));
	if (IsValid(LocalPlayerController) == true)
	{
		LocalPlayerController->TurnChangedDelegate.RemoveAll(this);
		LocalPlayerController->PlayerStateLoadedDelegate.RemoveAll(this);
	}
}

void UMainWidget::OnTurnChanged(bool bIsMyTurn)
{
	if (IsValid(Text_Turn) == true) 
	{
		Text_Turn->SetText(bIsMyTurn ? FText::FromString(TEXT("My Turn")) : FText::FromString(TEXT("Waiting")));
	}
}