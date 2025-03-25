// LINK

//Default
#include "PWGameMode_MainMenu.h"

//Engine
#include "Helper/PWGameplayStatics.h"

//Game
#include "Core/Subsystem/PWSteamMatchMakingSubsystem.h"
#include "PWGameState.h"

void APWGameMode_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	UPWSteamMatchMakingSubsystem* PWSteamMatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (ensure(IsValid(PWSteamMatchMakingSubsystem) == true))
	{
		PWSteamMatchMakingSubsystem->OnCreateSessionCompleteDelegate.AddUObject(this, &APWGameMode_MainMenu::OnCreateSessionComplete);
		PWSteamMatchMakingSubsystem->OnDestroySessionCompleteDelegate.AddUObject(this, &APWGameMode_MainMenu::OnDestroySessionComplete);
	}
}

void APWGameMode_MainMenu::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (bSessionCreated == true && UGameplayStatics::GetNumPlayerControllers(this) > 1)
	{
		// TODO: 테이블 값 참조해서 가져오기
		APWGameState* PWGameState = GetGameState<APWGameState>();
		if (ensure(IsValid(PWGameState) == true))
		{
			PWGameState->SetSelectedLevelKey(FName("Main"));
		}

		UE_LOG(LogTemp, Log, TEXT("Client login : %s"), *NewPlayer->GetName());
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
		world->ServerTravel(FString("/Game/Level/" + SelectedLevelKey.ToString() + FString("?listen")));
	}
}