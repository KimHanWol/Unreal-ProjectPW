// LINK

//Default
#include "MainWidget.h"

//Engine
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWGameSetting.h"
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
		PWEventManager->TeamCharacterMovedDelegate.AddUObject(this, &UMainWidget::OnTeamCharacterMoved);
		PWEventManager->TargetIsOnCrosshairDelegate.AddUObject(this, &UMainWidget::OnTargetIsonCrosshair);
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
		PWEventManager->TeamCharacterMovedDelegate.RemoveAll(this);
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
		Text_Result->SetVisibility(ESlateVisibility::Visible);
		Text_Result->SetText(bWon ? FText::FromString(TEXT("You Win")) : FText::FromString(TEXT("You Lose")));
	}
}

void UMainWidget::OnTeamCharacterMoved(float CurrentTurnActivePoint)
{
	float MaxTurnActivePoint = 0.f;
	const UPWGameSetting* GameSetting = UPWGameSetting::Get(this);
	if (IsValid(GameSetting) == true)
	{
		MaxTurnActivePoint = GameSetting->TurnActivePoint;
	}

	if (MaxTurnActivePoint <= 0.f)
	{
		ensure(false);
		return;
	}

	if (IsValid(ProgressBar_TurnPoint) == true)
	{
		ProgressBar_TurnPoint->SetPercent(CurrentTurnActivePoint / MaxTurnActivePoint);
	}
}

void UMainWidget::OnTargetIsonCrosshair(bool bIsOnCrosshair)
{
	if (IsValid(Text_Crosshair) == false)
	{
		return;
	}

	if (bIsOnCrosshair == true)
	{
		Text_Crosshair->SetColorAndOpacity(FSlateColor(FColor::Red));
	}
	else
	{
		Text_Crosshair->SetColorAndOpacity(FSlateColor(FColor::White));
	}
}
