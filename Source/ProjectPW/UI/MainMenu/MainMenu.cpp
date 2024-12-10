// LINK

//Default
#include "MainMenu.h"

//Engine
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Kismet/KismetSystemLibrary.h"

//Game
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"

void UMainMenu::BindEvents()
{
	Super::BindEvents();

	if (IsValid(Btn_CreateServer) == true)
	{
		Btn_CreateServer->OnPressed.AddDynamic(this, &UMainMenu::OnCreateButtonPressed);
	}

	if (IsValid(Btn_SearchServer) == true)
	{
		Btn_SearchServer->OnPressed.AddDynamic(this, &UMainMenu::OnSearchButtonPressed);
	}

	if (IsValid(Btn_Quit) == true)
	{
		Btn_Quit->OnPressed.AddDynamic(this, &UMainMenu::OnQuitButtonPressed);
	}
}

void UMainMenu::UnbindEvents()
{
	Super::UnbindEvents();

	if (IsValid(Btn_CreateServer) == true)
	{
		Btn_CreateServer->OnPressed.RemoveAll(this);
	}

	if (IsValid(Btn_SearchServer) == true)
	{
		Btn_SearchServer->OnPressed.RemoveAll(this);
	}

	if (IsValid(Btn_Quit) == true)
	{
		Btn_Quit->OnPressed.RemoveAll(this);
	}
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