// LINK

//Default
#include "PWSteamMatchMakingSubsystem.h"

//Engine
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWGameInstance.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Helper/PWGameplayStatics.h"

UPWSteamMatchMakingSubsystem::UPWSteamMatchMakingSubsystem()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem != nullptr)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

UPWSteamMatchMakingSubsystem* UPWSteamMatchMakingSubsystem::Get(const UObject* WorldContextObj)
{
    UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
    if (IsValid(PWGameInst) == true)
    {
        return PWGameInst->GetPWSteamMatchMakingSubsystem();
    }

    return nullptr;
}

void UPWSteamMatchMakingSubsystem::CreateGameSession()
{
	UE_LOG(LogTemp, Log, TEXT("Start create game session"));
    if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	FNamedOnlineSession* ExistSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistSession != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Remove exist session for create new one"));
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}

	const FOnCreateSessionCompleteDelegate& CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnCreateSessionComplete);
	CreateSessionCompleteHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	
	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
    SessionSettings->bIsLANMatch = false;
	SessionSettings->NumPublicConnections = 4;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
    const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (ensure(IsValid(LocalPlayer) == true))
	{
		OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
	}
}

void UPWSteamMatchMakingSubsystem::FindAndJoinGameSession()
{
	FindGameSession();
}

void UPWSteamMatchMakingSubsystem::LeaveGameSession()
{
	DestroyGameSession();

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerController* PWPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(PWPlayerController) == true)
	{
		PWPlayerController->ClientTravel(PWGameSetting->MainMenuLevelPath.ToString(), ETravelType::TRAVEL_Absolute);
	}
}

void UPWSteamMatchMakingSubsystem::FindGameSession()
{
	UE_LOG(LogTemp, Log, TEXT("Start find game session"));
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	const FOnFindSessionsCompleteDelegate& FindSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnFindSessionComplete);
	FindSessionCompleteHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true ,EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (ensure(IsValid(LocalPlayer) == true))
	{
		OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(),SessionSearch.ToSharedRef());
	}
}

void UPWSteamMatchMakingSubsystem::JoinGameSession()
{	
	UE_LOG(LogTemp, Log, TEXT("Start join game session"));
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	if(SessionSearch->SearchResults.Num() < 0)
	{
		ensure(false);
		return;
	}

	const FOnJoinSessionCompleteDelegate& JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnJoinSessionComplete);
	OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
	{
		FString Id = Result.GetSessionIdStr();
		FString USer = Result.Session.OwningUserName;
		FString MatchType;
		
		Result.Session.SessionSettings.Get(FName("MatchType"),MatchType);
		if(MatchType != FString("FreeForAll"))
		{
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("Match Type:%s / ID:%s, User:%s"), *MatchType, *Id, *USer);

		const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		if (ensure(IsValid(LocalPlayer) == true))
		{
			OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result);
			break;
		}
	}
}

void UPWSteamMatchMakingSubsystem::DestroyGameSession()
{
	UE_LOG(LogTemp, Log, TEXT("Start destroy game session"));
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	const FOnDestroySessionCompleteDelegate& DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnDestroySessionComplete);
	DestroySessionCompleteHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	OnlineSessionInterface->DestroySession(NAME_GameSession);
}

void UPWSteamMatchMakingSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return;
	}

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Create game session success"));

		UWorld* world = GetWorld();
		if (IsValid(world) == true)
		{
			world->ServerTravel(PWGameSetting->MainLevelPath.ToString() + FString("?listen"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Create game session failed"));
	}

	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);
	OnCreateSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UPWSteamMatchMakingSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("SearchResults : %d"), SessionSearch->SearchResults.Num());
	bWasSuccessful = bWasSuccessful == true && SessionSearch->SearchResults.Num() > 0;
	if(bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Find game session success"));

		const FOnJoinSessionCompleteDelegate& JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnJoinSessionComplete);
		OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

		JoinGameSession();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Find game session failed"));
	}

	OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteHandle);
	OnFindSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UPWSteamMatchMakingSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	FString SessionAddress;
	bool bWasSuccessful = OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, SessionAddress) && (SessionAddress.IsEmpty() == false);
	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Join game session success, session address : %s"), *SessionAddress);

		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (ensure(IsValid(PlayerController) == true))
		{
			PlayerController->ClientTravel(SessionAddress, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Join game session success, but can't get address from session."));
	}

	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteHandle);
	OnJoinSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UPWSteamMatchMakingSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Destroy game session success"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Destroy game session failed"));
	}

	OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteHandle);
	OnDestroySessionCompleteDelegate.Broadcast(bWasSuccessful);
}
