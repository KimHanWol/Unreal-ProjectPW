// LINK

//Default
#include "MainWidget.h"

//Engine
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Components/Overlay.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
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
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.AddUObject(this, &UMainWidget::OnGameOver);
		PWEventManager->TeamCharacterMovedDelegate.AddUObject(this, &UMainWidget::OnTeamCharacterMoved);
		PWEventManager->TargetIsOnCrosshairDelegate.AddUObject(this, &UMainWidget::OnTargetIsOnCrosshair);
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &UMainWidget::OnPlayerPossessed);
	}
}

void UMainWidget::UnbindEvents()
{
	Super::UnbindEvents();

	APWPlayerController* LocalPlayerController = Cast<APWPlayerController>(UPWGameplayStatics::GetLocalPlayerController(this));
	if (IsValid(LocalPlayerController) == true)
	{
		LocalPlayerController->TurnChangedDelegate.RemoveAll(this);
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->GameOverDelegate.RemoveAll(this);
		PWEventManager->TeamCharacterMovedDelegate.RemoveAll(this);
		PWEventManager->TargetIsOnCrosshairDelegate.RemoveAll(this);
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
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

void UMainWidget::OnTeamCharacterLoaded(ETeamSide TeamSide, const TArray<class APWPlayerCharacter*>& TeamCharcterList)
{
	if (UPWGameplayStatics::GetLocalPlayerTeamSide(this) != TeamSide)
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

void UMainWidget::TryInitializeCharacterData()
{
	//TODO: 폴링 방식 말고 이벤트 기반 방식으로 개선
	//데이터 초기화 브로드캐스트 이후 위젯이 바인딩해서 문제가 발생.
	if (bIsCharacterDataInitialized == true)
	{
		return;
	}

	APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
	if (IsValid(PWPlayerState) == true && PWPlayerState->IsTeamCharacterInitialized() == true) //완전히 초기화 되지 않은 상태의 접근 방지
	{
		OnTeamCharacterLoaded(UPWGameplayStatics::GetLocalPlayerTeamSide(this), PWPlayerState->GetTeamCharacterList());
		bIsCharacterDataInitialized = true;

		GetWorld()->GetTimerManager().ClearTimer(WaitCharacterDataInitTimerHandle);
	}
}
