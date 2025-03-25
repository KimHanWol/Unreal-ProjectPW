// LINK

//Default
#include "MainMenu.h"

//Engine
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Kismet/KismetSystemLibrary.h"

//Game
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataTable/PWLevelDataTableRow.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bIsAnimationPlaying = false;
	UpdateLevelData();
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
}

void UMainMenu::OnPreviousButtonPressed()
{
	if (bIsAnimationPlaying == true)
	{
		return;
	}

	CurrentSelectedLevelIndex = GetPrevLevelIndex();
	UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentSelectedLevelIndex);

	PlayAnimation(Anim_LevelSlide_Prev);
	bIsAnimationPlaying = true;

	float AnimationDuration = Anim_LevelSlide_Prev->GetEndTime();

	// 업데이트 중간에 이름 변경해야 해서 따로 분리
	FTimerHandle TimerMiddleHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerMiddleHandle,
		[this]()
		{
			UpdateLevelName();
		},
		AnimationDuration / 2.f,
		false
	);

	// 애니메이션 재생 이후에 데이터 변경
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			StopAnimation(Anim_LevelSlide_Prev); //처음 상태로 되돌림
			UpdateLevelData();
			bIsAnimationPlaying = false;
		},
		AnimationDuration,
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
	UE_LOG(LogTemp, Warning, TEXT("%d"), CurrentSelectedLevelIndex);

	PlayAnimation(Anim_LevelSlide_Next);
	bIsAnimationPlaying = true;

	float AnimationDuration = Anim_LevelSlide_Next->GetEndTime();

	// 업데이트 중간에 이름 변경해야 해서 따로 분리
	FTimerHandle TimerMiddleHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerMiddleHandle,
		[this]()
		{
			UpdateLevelName();
		},
		AnimationDuration / 2.f,
		false
	);

	// 애니메이션 재생 이후에 데이터 변경
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]()
		{
			StopAnimation(Anim_LevelSlide_Next); //처음 상태로 되돌림
			UpdateLevelData();
			bIsAnimationPlaying = false;
		},
		AnimationDuration,
		false
	);
}

void UMainMenu::OnCreateButtonPressed()
{
	if (IsValid(Text_Process) == true)
	{
		Text_Process->SetText(SessionCreatingText);
		Text_Process->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(Overlay_Btn) == true)
	{
		Overlay_Btn->SetVisibility(ESlateVisibility::Collapsed);
	}

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->CreateGameSession();
		PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.AddUObject(this, &UMainMenu::OnCreateSessionComplete);
	}
}

void UMainMenu::OnSearchButtonPressed()
{
	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->FindAndJoinGameSession();
		PWSteamMatchMakingSubsystem->OnFindSessionCompleteDelegate.AddUObject(this, &UMainMenu::OnFindSessionComplete);
		PWSteamMatchMakingSubsystem->OnJoinSessionCompleteDelegate.AddUObject(this, &UMainMenu::OnJoinSessionComplete);
	}

	if (IsValid(Text_Process) == true)
	{
		Text_Process->SetText(SessionSearchingText);
		Text_Process->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(Overlay_Btn) == true)
	{
		Overlay_Btn->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMenu::OnQuitButtonPressed()
{
	FGenericPlatformMisc::RequestExit(false);
}

void UMainMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.RemoveAll(this);
	}

	if (IsValid(Text_Process) == true)
	{
		Text_Process->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (IsValid(Overlay_Btn) == true)
	{
		Overlay_Btn->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenu::OnFindSessionComplete(bool bWasSuccessful)
{
	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->OnFindSessionCompleteDelegate.RemoveAll(this);
	}

	if (bWasSuccessful == true)
	{
		if (IsValid(Text_Process) == true)
		{
			Text_Process->SetText(SessionJoiningText);
			Text_Process->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (IsValid(MainMenuPopUp) == true)
		{
			MainMenuPopUp->SetVisibility(ESlateVisibility::Visible);
		}

		if (IsValid(Text_Process) == true)
		{
			Text_Process->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (IsValid(Overlay_Btn) == true)
		{
			Overlay_Btn->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainMenu::OnJoinSessionComplete(bool bWasSuccessful)
{
	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == true)
	{
		PWSteamMatchMakingSubsystem->OnJoinSessionCompleteDelegate.RemoveAll(this);
	}

	if (IsValid(Text_Process) == true)
	{
		Text_Process->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (bWasSuccessful == false)
	{
		if (IsValid(MainMenuPopUp) == true)
		{
			MainMenuPopUp->SetVisibility(ESlateVisibility::Visible);
		}

		if (IsValid(Overlay_Btn) == true)
		{
			Overlay_Btn->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainMenu::UpdateLevelName()
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

	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);
	if (ensure(LevelDataList.Num() > CurrentSelectedLevelIndex))
	{
		Text_LevelName->SetText(FText::FromName(LevelDataList[CurrentSelectedLevelIndex]->DisplayName));
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

	UE_LOG(LogTemp, Warning, TEXT("Data %d"), CurrentSelectedLevelIndex);

 	const TArray<FPWLevelDataTableRow*>& LevelDataList = PWGameData->GetAllTableRow<FPWLevelDataTableRow>(EDataTableType::Level);

	if (ensure(IsValid(Image_BG_Prev) == true))
	{
		int32 PrevLevelIndex = GetPrevLevelIndex();
		if (ensure(LevelDataList.Num() > PrevLevelIndex))
		{
			Image_BG_Prev->SetBrushFromSoftTexture(LevelDataList[PrevLevelIndex]->LevelImage);		
			UE_LOG(LogTemp, Warning, TEXT("Prev %d"), PrevLevelIndex);
		}
	}

	if (ensure(IsValid(Image_BG) == true))
	{
		if (ensure(LevelDataList.Num() > CurrentSelectedLevelIndex))
		{
			Image_BG_Prev->SetBrushFromSoftTexture(LevelDataList[CurrentSelectedLevelIndex]->LevelImage);
		}

		UE_LOG(LogTemp, Warning, TEXT("Curr %d"), CurrentSelectedLevelIndex);
	}

	if (ensure(IsValid(Image_BG) == true))
	{
		int32 NextLevelIndex = GetNextLevelIndex();
		if (ensure(LevelDataList.Num() > NextLevelIndex))
		{
			Image_BG_Prev->SetBrushFromSoftTexture(LevelDataList[NextLevelIndex]->LevelImage);
		}

		UE_LOG(LogTemp, Warning, TEXT("Next %d"), NextLevelIndex);
	}

	UpdateLevelName();
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

	return CurrentSelectedLevelIndex - 1 < 0 ? LevelDataList.Num() - 1 : CurrentSelectedLevelIndex - 1;
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

	return CurrentSelectedLevelIndex + 1 >= LevelDataList.Num() ? 0 : CurrentSelectedLevelIndex + 1;
}
