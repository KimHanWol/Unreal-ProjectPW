﻿// LINK

//Default
#include "MainMenu.h"

//Engine
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//Game
#include "Core/PWEventManager.h"
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataAsset/PWSoundData.h"
#include "Data/DataTable/PWLevelDataTableRow.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsAnimationPlaying = false;
	UpdateLevelData();

	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(LocalPlayerController) == false)
	{
		ensure(false);
		return;
	}

	// 세션 생성 중 이거나 게임에 참여해 클라이언트면
	ENetMode NetMode = GetWorld()->GetNetMode();
	if ((int32)NetMode == 2 || LocalPlayerController->HasAuthority() == false)
	{
		if (ensure(IsValid(Btn_CreateSession) == true))
		{
			Btn_CreateSession->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (ensure(IsValid(Btn_SearchSession) == true))
		{
			Btn_SearchSession->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (ensure(IsValid(Btn_Quit) == true))
		{
			Btn_Quit->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 처음 들어왔으면
	else
	{
		if (ensure(IsValid(Btn_CreateSession) == true))
		{
			Btn_CreateSession->SetVisibility(ESlateVisibility::Visible);
		}

		if (ensure(IsValid(Btn_SearchSession) == true))
		{
			Btn_SearchSession->SetVisibility(ESlateVisibility::Visible);
		}

		if (ensure(IsValid(Btn_Quit) == true))
		{
			Btn_Quit->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainMenu::BindEvents()
{
	Super::BindEvents();

	if (ensure(IsValid(Btn_Previous) == true))
	{
		Btn_Previous->OnPressed.AddDynamic(this, &UMainMenu::OnPreviousButtonPressed);
	}

	if (ensure(IsValid(Btn_Next) == true))
	{
		Btn_Next->OnPressed.AddDynamic(this, &UMainMenu::OnNextButtonPressed);
	}

	if (ensure(IsValid(Btn_CreateSession) == true))
	{
		Btn_CreateSession->OnPressed.AddDynamic(this, &UMainMenu::OnCreateButtonPressed);
	}

	if (ensure(IsValid(Btn_SearchSession) == true))
	{
		Btn_SearchSession->OnPressed.AddDynamic(this, &UMainMenu::OnSearchButtonPressed);
	}

	if (ensure(IsValid(Btn_Quit) == true))
	{
		Btn_Quit->OnPressed.AddDynamic(this, &UMainMenu::OnQuitButtonPressed);
	}

	if (ensure(IsValid(Btn_StopMatchMaking) == true))
	{
		Btn_StopMatchMaking->OnPressed.AddDynamic(this, &UMainMenu::OnStopMatchMakingButtonPressed);
	}

	FWidgetAnimationDynamicEvent SlideAnimEvent;
	SlideAnimEvent.BindDynamic(this, &UMainMenu::OnLevelSlideAnimFinished);
	BindToAnimationFinished(Anim_LevelSlide_Prev, SlideAnimEvent);
	BindToAnimationFinished(Anim_LevelSlide_Next, SlideAnimEvent);

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.AddUObject(this, &UMainMenu::OnCreateSessionComplete);
		PWSteamMatchMakingSubsystem->OnFindSessionCompleteDelegate.AddUObject(this, &UMainMenu::OnFindSessionComplete);
		PWSteamMatchMakingSubsystem->OnJoinSessionCompleteDelegate.AddUObject(this, &UMainMenu::OnJoinSessionComplete);
	}
}

void UMainMenu::UnbindEvents()
{
	Super::UnbindEvents();

	if (IsValid(Btn_CreateSession) == true)
	{
		Btn_CreateSession->OnPressed.RemoveAll(this);
	}

	if (IsValid(Btn_SearchSession) == true)
	{
		Btn_SearchSession->OnPressed.RemoveAll(this);
	}

	if (IsValid(Btn_Quit) == true)
	{
		Btn_Quit->OnPressed.RemoveAll(this);
	}

	UnbindAllFromAnimationFinished(Anim_LevelSlide_Prev);
	UnbindAllFromAnimationFinished(Anim_LevelSlide_Next);

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.RemoveAll(this);
		PWSteamMatchMakingSubsystem->OnFindSessionCompleteDelegate.RemoveAll(this);
		PWSteamMatchMakingSubsystem->OnJoinSessionCompleteDelegate.RemoveAll(this);
	}
}

void UMainMenu::OnPreviousButtonPressed()
{
	if (bIsAnimationPlaying == true)
	{
		return;
	}

	CurrentSelectedLevelIndex = GetPrevLevelIndex();

	PlayAnimation(Anim_LevelSlide_Prev, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, true);

	bIsAnimationPlaying = true;

	float AnimationDuration = Anim_LevelSlide_Prev->GetEndTime();

	// 업데이트 중간에 이름 변경해야 해서 따로 분리
	FTimerHandle TimerMiddleHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerMiddleHandle,
		[this]()
		{
			UpdateCurrentLevelData();
		},
		AnimationDuration / 2.f,
		false
	);
}

void UMainMenu::OnNextButtonPressed()
{
	if (bIsAnimationPlaying == true)
	{
		return;
	}

	CurrentSelectedLevelIndex = GetNextLevelIndex();

	PlayAnimation(Anim_LevelSlide_Next, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, true);
	bIsAnimationPlaying = true;

	float AnimationDuration = Anim_LevelSlide_Next->GetEndTime();

	// 업데이트 중간에 이름 변경해야 해서 따로 분리
	FTimerHandle TimerMiddleHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerMiddleHandle,
		[this]()
		{
			UpdateCurrentLevelData();
		},
		AnimationDuration / 2.f,
		false
	);
}

void UMainMenu::OnCreateButtonPressed()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);
	if (LevelDataList.Num() < CurrentSelectedLevelIndex)
	{
		ensure(false);
		return;
	}

	// Cancel with in develpoment level
	if (LevelDataList[CurrentSelectedLevelIndex]->bIsInDevelopment == true)
	{
		return;
	}

	TSoftObjectPtr<UDataTable> DataTable = PWGameData->GetDataTable(EDataTableType::Level);
	if (DataTable.IsNull() == true)
	{
		ensure(false);
		return;
	}

	TArray<FName> DataTableKeyList = DataTable.LoadSynchronous()->GetRowNames();
	if (DataTableKeyList.Num() <= CurrentSelectedLevelIndex)
	{
		ensure(false);
		return;
	}

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->CreateGameSession(DataTableKeyList[CurrentSelectedLevelIndex]);
	}

	EnableWaitingUI(SessionCreatingText);

	BP_PlayClickSound();

	bIsMatchMaking = true;
}

void UMainMenu::OnSearchButtonPressed()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);
	if (LevelDataList.Num() < CurrentSelectedLevelIndex)
	{
		ensure(false);
		return;
	}

	// Cancel with in develpoment level
	if (LevelDataList[CurrentSelectedLevelIndex]->bIsInDevelopment == true)
	{
		return;
	}

	TSoftObjectPtr<UDataTable> DataTable = PWGameData->GetDataTable(EDataTableType::Level);
	if (DataTable.IsNull() == true)
	{
		ensure(false);
		return;
	}

	TArray<FName> DataTableKeyList = DataTable.LoadSynchronous()->GetRowNames();
	if (DataTableKeyList.Num() <= CurrentSelectedLevelIndex)
	{
		ensure(false);
		return;
	}

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->FindAndJoinGameSession(DataTableKeyList[CurrentSelectedLevelIndex]);
	}

	EnableWaitingUI(SessionSearchingText);

	BP_PlayClickSound();

	bIsMatchMaking = true;
}

void UMainMenu::OnQuitButtonPressed()
{
	BP_PlayClickSound();
	FGenericPlatformMisc::RequestExit(false);
}

void UMainMenu::OnStopMatchMakingButtonPressed()
{
	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->StopMatchMaking();
	}

	DisableWaitingUI();

	BP_PlayClickSound();

	bIsMatchMaking = false;
}

void UMainMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	if(bWasSuccessful == false)
	{
		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (ensure(IsValid(PWEventManager) == true))
		{
			PWEventManager->ShowNotiWidgetDelegate.Broadcast(CreateSessionFailedText);
		}

		DisableWaitingUI();

		PlayNotificationSound();

		bIsMatchMaking = false;
	}
}

void UMainMenu::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful == true)
	{
		EnableWaitingUI(SessionJoiningText);
	}
	else
	{
		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (ensure(IsValid(PWEventManager) == true))
		{
			PWEventManager->ShowNotiWidgetDelegate.Broadcast(CannotFindSuitableSessionText);
		}

		DisableWaitingUI();

		PlayNotificationSound();

		bIsMatchMaking = false;
	}
}

void UMainMenu::OnJoinSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful == false)
	{
		bIsMatchMaking = false;
	}
}

void UMainMenu::UpdateCurrentLevelData()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Text_LevelName) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Btn_CreateSession) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Btn_SearchSession) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);
	if (LevelDataList.Num() <= CurrentSelectedLevelIndex)
	{
		ensure(false);
		return;
	}

	const FPWLevelDataTableRow* LevelData = LevelDataList[CurrentSelectedLevelIndex];
	if (LevelData == nullptr)
	{
		ensure(false);
		return;
	}

	Text_LevelName->SetText(FText::FromName(LevelData->DisplayName));
	if (LevelData->bIsInDevelopment == true)
	{
		Btn_CreateSession->SetVisibility(ESlateVisibility::Hidden);
		Btn_SearchSession->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Btn_CreateSession->SetVisibility(ESlateVisibility::Visible);
		Btn_SearchSession->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenu::UpdateLevelData()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

 	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);

	if (ensure(IsValid(Image_BG_Prev) == true))
	{
		int32 PrevLevelIndex = GetPrevLevelIndex();
		if (ensure(LevelDataList.Num() > PrevLevelIndex))
		{
			Image_BG_Prev->SetBrushFromSoftTexture(LevelDataList[PrevLevelIndex]->LevelImage.LoadSynchronous());		
		}
	}

	if (ensure(IsValid(Image_BG) == true))
	{
		if (ensure(LevelDataList.Num() > CurrentSelectedLevelIndex))
		{
			Image_BG->SetBrushFromSoftTexture(LevelDataList[CurrentSelectedLevelIndex]->LevelImage.LoadSynchronous());
		}
	}

	if (ensure(IsValid(Image_BG_Next) == true))
	{
		int32 NextLevelIndex = GetNextLevelIndex();
		if (ensure(LevelDataList.Num() > NextLevelIndex))
		{
			Image_BG_Next->SetBrushFromSoftTexture(LevelDataList[NextLevelIndex]->LevelImage.LoadSynchronous());
		}
	}

	UpdateCurrentLevelData();
}


int32 UMainMenu::GetPrevLevelIndex()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return 0;
	}

	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);

	int32 NewLevelIndex = CurrentSelectedLevelIndex;
	do
	{
		NewLevelIndex = NewLevelIndex - 1 < 0 ? LevelDataList.Num() - 1 : NewLevelIndex - 1;

		// Dev 맵은 공개하지 않음
		if (LevelDataList.Num() > NewLevelIndex && LevelDataList[NewLevelIndex] != nullptr && LevelDataList[NewLevelIndex]->bIsDevMap == false)
		{
			break;
		}

	} while (NewLevelIndex != CurrentSelectedLevelIndex);

	return NewLevelIndex;
}

int32 UMainMenu::GetNextLevelIndex()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return 0;
	}

	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);

	int32 NewLevelIndex = CurrentSelectedLevelIndex;
	do
	{
		NewLevelIndex = NewLevelIndex + 1 >= LevelDataList.Num() ? 0 : NewLevelIndex + 1;

		// Dev 맵은 공개하지 않음
		if (ensure(LevelDataList.Num() > NewLevelIndex) && LevelDataList[NewLevelIndex]->bIsDevMap == false)
		{
			break;
		}

	} while (NewLevelIndex != CurrentSelectedLevelIndex);

	return NewLevelIndex;
}

void UMainMenu::OnLevelSlideAnimFinished()
{
	UpdateLevelData();
	bIsAnimationPlaying = false;
}

void UMainMenu::PlayNotificationSound()
{
	const UPWSoundData* SoundData = UPWSoundData::Get(this);
	if (ensure(SoundData->NotificationWidgetSound.IsNull() == false))
	{
		UGameplayStatics::PlaySound2D(this, SoundData->NotificationWidgetSound.LoadSynchronous());
	}
}

void UMainMenu::EnableWaitingUI(const FText& WaitingText)
{
	if (ensure(IsValid(Text_Process) == true))
	{
		Text_Process->SetText(WaitingText);
	}

	if (ensure(IsValid(Overlay_Main) == true))
	{
		Overlay_Main->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ensure(IsValid(Overlay_Waiting) == true))
	{
		Overlay_Waiting->SetVisibility(ESlateVisibility::Visible);
	}

	if (ensure(IsValid(Anim_MatchMaking) == true))
	{
		PlayAnimation(Anim_MatchMaking, 0.f, 0);
	}
}

void UMainMenu::DisableWaitingUI()
{
	if (ensure(IsValid(Overlay_Main) == true))
	{
		Overlay_Main->SetVisibility(ESlateVisibility::Visible);
	}

	if (ensure(IsValid(Overlay_Waiting) == true))
	{
		Overlay_Waiting->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ensure(IsValid(Anim_MatchMaking) == true))
	{
		StopAnimation(Anim_MatchMaking);
	}
}
