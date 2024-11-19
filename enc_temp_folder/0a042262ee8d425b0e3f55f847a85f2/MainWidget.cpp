// LINK

//Default
#include "MainWidget.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.AddUObject(this, &UMainWidget::OnGameOver);
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.RemoveAll(this);
	}
}

void UMainWidget::OnTurnChanged(bool bIsMyTurn)
{
	if (IsValid(Text_Turn) == true) 
	{
		Text_Turn->SetText(bIsMyTurn ? FText::FromString(TEXT("My Turn")) : FText::FromString(TEXT("Waiting")));
	}
}

void UMainWidget::OnGameOver(bool bWon)
{
	if (IsValid(Text_Result) == true)
	{
		Text_Result->SetText(bWon ? FText::FromString(TEXT("You Win")) : FText::FromString(TEXT("You Lose")));
	}
}
