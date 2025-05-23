﻿// LINK

//Default
#include "MainWidget.h"

//Engine
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
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

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (GetVisibility() == ESlateVisibility::Hidden || GetVisibility() == ESlateVisibility::Collapsed)
	{
		return;
	}

	if (IsValid(ProgressBar_TurnPoint_Character) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(ProgressBar_TurnPoint_Character_Back) == false)
	{
		ensure(false);
		return;
	}

	float CurrentTurnActivePointPercent = ProgressBar_TurnPoint_Character->GetPercent();

	// 부드럽게 줄어들게 Lerp 또는 Interp
	TurnActivePointDecreaseStartPercent = FMath::FInterpTo(TurnActivePointDecreaseStartPercent, CurrentTurnActivePointPercent, InDeltaTime, 5.f);
	ProgressBar_TurnPoint_Character_Back->SetPercent(TurnActivePointDecreaseStartPercent);
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
		PWEventManager->CharacterTriedToUseSkillDelegate.AddUObject(this, &UMainWidget::OnCharacterTriedToUseSkill);
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
		PWEventManager->CharacterTriedToUseSkillDelegate.RemoveAll(this);
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

	if (ensure(IsValid(ProgressBar_TurnPoint_Commander) == true))
	{
		ProgressBar_TurnPoint_Commander->SetPercent(CurrentTurnActivePoint / MaxTurnActivePoint);
	}

	if (ensure(IsValid(ProgressBar_TurnPoint_Character) == true))
	{
		ProgressBar_TurnPoint_Character->SetPercent(CurrentTurnActivePoint / MaxTurnActivePoint);
	}
}

void UMainWidget::OnTeamCharacterAllSpawned(const APWPlayerController* TargetPlayerController, const TArray<TWeakObjectPtr<class APWPlayerCharacter>>& TargetCharacterList)
{
	if (UPWGameplayStatics::GetLocalPlayerController(this) != TargetPlayerController)
	{
		return;
	}

	for (int32 i = 0; i < CharacterButtonList.Num(); i++)
	{
		UMainWidget_CharacterButton* MainWidget_CharacterButton = CharacterButtonList[i];
		if (TargetCharacterList.Num() > i)
		{
			MainWidget_CharacterButton->InitializeCharacterButton(TargetCharacterList[i].Get());
		}
	}

	if (ensure(IsValid(Text_TeamSide) == true))
	{
		if (TargetPlayerController->GetTeamSide() == ETeamSide::Red)
		{
			Text_TeamSide->SetText(FText::FromString(TEXT("Red Team")));
			Text_TeamSide->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f));
		}
		else if(TargetPlayerController->GetTeamSide() == ETeamSide::Blue)
		{
			Text_TeamSide->SetText(FText::FromString(TEXT("Blue Team")));
			Text_TeamSide->SetColorAndOpacity(FLinearColor(0.f, 0.f, 1.f));
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

void UMainWidget::OnCharacterTriedToUseSkill(float TurnActivePointCost, bool bIsSuccessed)
{
	if (IsValid(Overlay_TurnPoint_Character) == false)
	{
		ensure(false);
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(TurnPointProgressBarShakeTimerHandle);
	Overlay_TurnPoint_Character->SetRenderTranslation(FVector2D(0.f, 0.f));

	if (bIsSuccessed == false)
	{
		GetWorld()->GetTimerManager().SetTimer(TurnPointProgressBarShakeTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this, Elapsed = 0.f]() mutable
			{
				Elapsed += 0.01f;

				float OffsetX = FMath::Sin(Elapsed * 100.f) * 5.f;
				float OffsetY = FMath::Cos(Elapsed * 150.f) * 5.f;

				Overlay_TurnPoint_Character->SetRenderTranslation(FVector2D(OffsetX, OffsetY));

				if (Elapsed >= 0.3f)
				{
					Overlay_TurnPoint_Character->SetRenderTranslation(FVector2D(0.f, 0.f));
					GetWorld()->GetTimerManager().ClearTimer(TurnPointProgressBarShakeTimerHandle);
					return;
				}

			}), 0.01f, true);
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
