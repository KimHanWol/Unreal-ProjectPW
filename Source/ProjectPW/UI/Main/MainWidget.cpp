// LINK

//Default
#include "MainWidget.h"

//Engine
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Components/Overlay.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Data/PWGameEnum.h"
#include "Helper/PWGameplayStatics.h"
#include "UI/Main/MainWidget_CharacterButton.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnPlayerPossessed(nullptr, true);

	if (IsValid(MainWidget_CharacterButton_1) == true)
	{
		CharacterButtonList.Add(MainWidget_CharacterButton_1);
	}

	if (IsValid(MainWidget_CharacterButton_2) == true)
	{
		CharacterButtonList.Add(MainWidget_CharacterButton_2);
	}

	if (IsValid(MainWidget_CharacterButton_3) == true)
	{
		CharacterButtonList.Add(MainWidget_CharacterButton_3);
	}

	if (IsValid(MainWidget_CharacterButton_4) == true)
	{
		CharacterButtonList.Add(MainWidget_CharacterButton_4);
	}

	if (IsValid(MainWidget_CharacterButton_5) == true)
	{
		CharacterButtonList.Add(MainWidget_CharacterButton_5);
	}

	if (IsValid(GetWorld()) == true)
	{
		GetWorld()->GetTimerManager().SetTimer(WaitCharacterDataInitTimerHandle, this, &UMainWidget::TryInitializeCharacterData, 0.01f, true);
	}
}

void UMainWidget::InvalidateWidget()
{
	Super::InvalidateWidget();
	OnTurnChanged();
}

void UMainWidget::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.AddUObject(this, &UMainWidget::OnGameOver);
		PWEventManager->TeamCharacterMovedDelegate.AddUObject(this, &UMainWidget::OnTeamCharacterMoved);
		PWEventManager->TargetIsOnCrosshairDelegate.AddUObject(this, &UMainWidget::OnTargetIsOnCrosshair);
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &UMainWidget::OnPlayerPossessed);
		PWEventManager->TurnChangedDelegate.AddUObject(this, &UMainWidget::OnTurnChanged);
		PWEventManager->TeamCharacterAllSpawnedDelegate.AddUObject(this, &UMainWidget::OnTeamCharacterAllSpawned);
	}

	if (IsValid(Btn_ReturnMainMenu) == true)
	{
		Btn_ReturnMainMenu->OnPressed.AddDynamic(this, &UMainWidget::OnReturnToMainMenuButtonPressed);
	}
}

void UMainWidget::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.RemoveAll(this);
		PWEventManager->TeamCharacterMovedDelegate.RemoveAll(this);
		PWEventManager->TargetIsOnCrosshairDelegate.RemoveAll(this);
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
		PWEventManager->TurnChangedDelegate.RemoveAll(this);
		PWEventManager->TeamCharacterAllSpawnedDelegate.RemoveAll(this);
	}
}

void UMainWidget::OnGameOver(APWPlayerController* PlayerController, bool bLose)
{
	if (PlayerController->IsLocalPlayerController() == false)
	{
		return;
	}

	if (IsValid(Overlay_Result) == true)
	{
		Overlay_Result->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(Text_Result) == true)
	{
		Text_Result->SetText(bLose ? FText::FromString(TEXT("You Lose")) : FText::FromString(TEXT("You Win")));
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

void UMainWidget::OnTeamCharacterAllSpawned(const APWPlayerController* TargetPlayerController, const TArray<class APWPlayerCharacter*>& TeamCharcterList)
{
	if (UPWGameplayStatics::GetLocalPlayerController(this) != TargetPlayerController)
	{
		return;
	}

	for (int32 i = 0; i < CharacterButtonList.Num(); i++)
	{
		UMainWidget_CharacterButton* MainWidget_CharacterButton = CharacterButtonList[i];
		if (TeamCharcterList.Num() > i)
		{
			MainWidget_CharacterButton->InitializeCharacterButton(TeamCharcterList[i]);
		}
	}
}

void UMainWidget::OnTargetIsOnCrosshair(APWPlayerCharacter* AimingCharcter, bool bIsOnCrosshair)
{
	if (IsValid(Image_Crosshair) == false)
	{
		return;
	}
	
	if (bIsOnCrosshair == true)
	{
		Image_Crosshair->SetColorAndOpacity(FColor::Red);
	}
	else
	{
		Image_Crosshair->SetColorAndOpacity(FColor::White);
	}
}

void UMainWidget::OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander)
{
	if (IsValid(Overlay_Commander) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Overlay_Character) == false)
	{
		ensure(false);
		return;
	}

	if (bIsCommander == true)
	{
		Overlay_Commander->SetVisibility(ESlateVisibility::Visible);
		Overlay_Character->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		Overlay_Commander->SetVisibility(ESlateVisibility::Collapsed);
		Overlay_Character->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainWidget::OnTurnChanged()
{
	bool bMyTurn = false;
	const APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
	if (IsValid(PWPlayerState) == true)
	{
		bMyTurn = PWPlayerState->IsMyTurn();		
	}

	if (IsValid(Text_Turn) == true)
	{
		if (bMyTurn == true)
		{
			Text_Turn->SetText(FText::FromString(TEXT("My Turn")));
		}
		else
		{
			Text_Turn->SetText(FText::FromString(TEXT("Waiting")));
		}
	}
}

void UMainWidget::OnReturnToMainMenuButtonPressed()
{
	UPWSteamMatchMakingSubsystem* MatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(MatchMakingSubsystem) == true)
	{
		MatchMakingSubsystem->LeaveGameSession();
	}
}

void UMainWidget::TryInitializeCharacterData()
{
	//TODO: 폴링 방식 말고 이벤트 기반 방식으로 개선
	//데이터 초기화 브로드캐스트 이후 위젯이 바인딩해서 문제가 발생.
	APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
	if (IsValid(PWPlayerState) == true && PWPlayerState->IsTeamCharacterInitialized() == true) //완전히 초기화 되지 않은 상태의 접근 방지
	{
		OnTeamCharacterAllSpawned(UPWGameplayStatics::GetLocalPlayerController(this), PWPlayerState->GetTeamCharacterList());
		GetWorld()->GetTimerManager().ClearTimer(WaitCharacterDataInitTimerHandle);
	}
}
