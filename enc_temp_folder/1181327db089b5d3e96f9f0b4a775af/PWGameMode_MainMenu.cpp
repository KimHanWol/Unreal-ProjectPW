// LINK

//Default
#include "PWGameMode_MainMenu.h"

//Engine
#include "Helper/PWGameplayStatics.h"

//Game
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "PWGameInstance.h"
#include "PWGameState.h"

void APWGameMode_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(PWSteamMatchMakingSubsystem) == false)
	{
		ensure(false);
		return;
	}

	UPWGameInstance* GameInst = UPWGameInstance::Get(this);
	check(GameInst);

	PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.RemoveAll(this);
	PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.AddUObject(this, &APWGameMode_MainMenu::OnCreateSessionComplete);
	PWSteamMatchMakingSubsystem->OnDestroySessionCompleteDelegate.RemoveAll(this);
	PWSteamMatchMakingSubsystem->OnDestroySessionCompleteDelegate.AddUObject(this, &APWGameMode_MainMenu::OnDestroySessionComplete);

	PWSteamMatchMakingSubsystem->LeaveGameSession();

	// CreateSession을 눌러 listen 모드로 변환 후
	ENetMode NetMode = GetWorld()->GetNetMode();
	if ((int32)NetMode == 2)
	{
		PWSteamMatchMakingSubsystem->CreateGameSession(GameInst->SelectedInGameLevelKey);
	}
}

void APWGameMode_MainMenu::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	UPWSteamMatchMakingSubsystem* MatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(MatchMakingSubsystem) == false)
	{
		ensure(false);
		return;
	}

	if (bSessionCreated == true && UGameplayStatics::GetNumPlayerControllers(this) > 1)
	{
		UE_LOG(LogTemp, Log, TEXT("All client login."));

		MatchMakingSubsystem->StartGameSession();
		TravelInGameLevel();
	}
}

void APWGameMode_MainMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful == true)
	{
		bSessionCreated = true;
	}
}

void APWGameMode_MainMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful == true)
	{
		bSessionCreated = false;
	}
}

void APWGameMode_MainMenu::TravelInGameLevel()
{
	UWorld* world = GetWorld();
	if (ensure(IsValid(world) == true))
	{
		FName SelectedLevelKey = UPWGameplayStatics::GetSelectedLevelKey(this);
		UE_LOG(LogTemp, Log, TEXT("Trying to travel for InGame level : %s"), *SelectedLevelKey.ToString());
		world->ServerTravel(FString("/Game/Level/InGame/" + SelectedLevelKey.ToString() + FString("?listen")));
	}
}