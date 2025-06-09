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
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Helper/PWGameplayStatics.h"
#include "UI/MasterWidget.h"

UPWSteamMatchMakingSubsystem::UPWSteamMatchMakingSubsystem()
{
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UPWSteamMatchMakingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	const FOnSessionUserInviteAcceptedDelegate& UserInviteAcceptedCompleteDelegate = FOnSessionUserInviteAcceptedDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnUserInviteAccepted);
	UserInviteAcceptedHandle = OnlineSessionInterface->AddOnSessionUserInviteAcceptedDelegate_Handle(UserInviteAcceptedCompleteDelegate);
}

void UPWSteamMatchMakingSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	OnlineSessionInterface->ClearOnSessionUserInviteAcceptedDelegate_Handle(UserInviteAcceptedHandle);
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

void UPWSteamMatchMakingSubsystem::CreateGameSession(FName InSelectedLevelKey)
{
	//레벨 등록
	UPWGameInstance* GameInst = UPWGameInstance::Get(this);
	check(GameInst);

	GameInst->SelectedInGameLevelKey = InSelectedLevelKey;

	// NetMode 확인, listen Mode로 변환 후 GameMode 에서 다시 CreateGameSession 호출
	ENetMode NetMode = GetWorld()->GetNetMode();
	if ((int32)NetMode != 2)
	{
		// 사운드 재생을 위해 레벨이동 딜레이
		FTimerHandle DelayHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateWeakLambda(this, [this]() 
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu?listen"), true);
			}), 0.1f, false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("NetMode : %d"), (int32)NetMode);
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
	SessionSettings->bAllowInvites = true;
	SessionSettings->NumPublicConnections = 1;
	SessionSettings->bAllowJoinInProgress = false;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(FName("LevelKey"), InSelectedLevelKey.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (IsValid(LocalPlayer) == false)
	{
		ensure(false);
		return;
	}

	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *SessionSettings);
}

void UPWSteamMatchMakingSubsystem::StartGameSession()
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	OnlineSessionInterface->StartSession(NAME_GameSession);
}

void UPWSteamMatchMakingSubsystem::FindAndJoinGameSession(FName InSelectedLevelKey)
{
	FindGameSession(InSelectedLevelKey);
}

void UPWSteamMatchMakingSubsystem::StopMatchMaking()
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	// Create Session
	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);

	//Find Session
	OnlineSessionInterface->CancelFindSessions();
	OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteHandle);

	//Joining Session
	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteHandle);

	// 사운드 재생을 위해 딜레이
	FTimerHandle DelayHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
		{
			DestroyGameSession();
		}), 0.1f, false);
}

void UPWSteamMatchMakingSubsystem::LeaveGameSession()
{
	DestroyGameSession();
}

void UPWSteamMatchMakingSubsystem::ClientTravelToSessionLevel(FName SessionName)
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	FString ConnectInfo;
	if (OnlineSessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
	{
		APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (ensure(IsValid(LocalPlayerController) == true))
		{
			UE_LOG(LogTemp, Log, TEXT("Trying to travel for server level."));
			LocalPlayerController->PreClientTravel(ConnectInfo, TRAVEL_Absolute, true);
			LocalPlayerController->ClientTravel(ConnectInfo, TRAVEL_Absolute);
		}
	}
}

void UPWSteamMatchMakingSubsystem::FindGameSession(FName InSelectedLevelKey)
{
	UE_LOG(LogTemp, Log, TEXT("Start find game session"));
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	UPWGameInstance* GameInst = UPWGameInstance::Get(this);
	check(GameInst);

	GameInst->SelectedInGameLevelKey = InSelectedLevelKey;

	const FOnFindSessionsCompleteDelegate& FindSessionCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnFindSessionComplete);
	FindSessionCompleteHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	//SessionSearch->QuerySettings.Set(FName("LevelKey"), InSelectedLevelKey.ToString(), EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (ensure(IsValid(LocalPlayer) == true))
	{
		OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
	}
}

void UPWSteamMatchMakingSubsystem::JoinGameSession(const TArray<FOnlineSessionSearchResult>& SearchResults)
{	
	UE_LOG(LogTemp, Log, TEXT("Start join game session"));
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	if(SearchResults.Num() < 1)
	{
		ensure(false);
		return;
	}

	const FOnJoinSessionCompleteDelegate& JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnJoinSessionComplete);
	JoinSessionCompleteHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	//TODO: Ping 확인해서 제일 적은 순으로 정렬 후 접근
	for (const FOnlineSessionSearchResult& Result : SearchResults)
	{
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

	if (OnlineSessionInterface->GetSessionState(NAME_GameSession) == EOnlineSessionState::Destroying)
	{
		return;
	}

	const FOnDestroySessionCompleteDelegate& DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnDestroySessionComplete);
	DestroySessionCompleteHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	OnlineSessionInterface->DestroySession(NAME_GameSession);
}

void UPWSteamMatchMakingSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Create game session success"));

		const FOnStartSessionCompleteDelegate StartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UPWSteamMatchMakingSubsystem::OnStartSessionComplete);
		StartSessionCompleteHandle = OnlineSessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Create game session failed"));

		UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"), true);
	}

	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteHandle);

	OnCreateSessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UPWSteamMatchMakingSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	UPWGameInstance* GameInst = UPWGameInstance::Get(this);
	check(GameInst);

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Start session success, current state : %s"), EOnlineSessionState::ToString(OnlineSessionInterface->GetSessionState(SessionName)));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Start session failed."));
	}

	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(StartSessionCompleteHandle);
}

void UPWSteamMatchMakingSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Log, TEXT("Find session was successful : %s"), bWasSuccessful == true ? *FString("True") : *FString("False"));
	UE_LOG(LogTemp, Log, TEXT("SearchResults : %d"), SessionSearch->SearchResults.Num());

	TArray<FOnlineSessionSearchResult> SearchResults;

	// 조건 한번 더 체크
	UPWGameInstance* GameInst = UPWGameInstance::Get(this);
	check(GameInst);

	UE_LOG(LogTemp, Warning, TEXT("Searching : LevelKey %s"), *GameInst->SelectedInGameLevelKey.ToString());

	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		FString LevelKeyString;
		SearchResult.Session.SessionSettings.Get(FName("LevelKey"), LevelKeyString);

		UE_LOG(LogTemp, Warning, TEXT("SearchResult : LevelKey %s"), *LevelKeyString);

		if (LevelKeyString == GameInst->SelectedInGameLevelKey.ToString())
		{
			SearchResults.Add(SearchResult);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("SearchResults after condition : %d"), SearchResults.Num());

	JoinRetryCount = JoinRetryMaxCount;

	bool bFindSessionSuccessful = bWasSuccessful == true && SearchResults.Num() > 0;
	if(bFindSessionSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Find game session success"));

		JoinGameSession(SearchResults);

		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteHandle);
		OnFindSessionCompleteDelegate.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Find game session failed"));

		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteHandle);
		OnFindSessionCompleteDelegate.Broadcast(bFindSessionSuccessful);
	}
}

void UPWSteamMatchMakingSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSessionInterface.IsValid() == false)
	{
		ensure(false);
		return;
	}

	FString SessionAddress;
	OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, SessionAddress);

	int32 NumPublicConnections = OnlineSessionInterface->GetNamedSession(SessionName)->SessionSettings.NumPublicConnections;
	EOnlineSessionState::Type OnlineSessionState = OnlineSessionInterface->GetNamedSession(SessionName)->SessionState;

	UE_LOG(LogTemp, Log, TEXT("[SESSION NAME] %s"), *SessionName.ToString());
	UE_LOG(LogTemp, Log, TEXT("Public connection : %d"), NumPublicConnections);
	UE_LOG(LogTemp, Log, TEXT("Session State : %s"), EOnlineSessionState::ToString(OnlineSessionState));

	bool bWasSuccessful =
		OnlineSessionState == EOnlineSessionState::Pending &&
		Result == EOnJoinSessionCompleteResult::Success &&
		SessionAddress.IsEmpty() == false &&
		NumPublicConnections > 0;

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Join game session success, session address : %s"), *SessionAddress);

		ClientTravelToSessionLevel(SessionName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Join game session success, but it was not available session."));
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

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (IsValid(PlayerController) == false)
	{
		ensure(false);
		return;
	}

	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Destroy game session success"));

		PlayerController->ClientTravel(PWGameSetting->MainMenuLevelPath.ToString(), ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy game session failed"));
	}

	OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteHandle);

	OnDestroySessionCompleteDelegate.Broadcast(bWasSuccessful);
}

void UPWSteamMatchMakingSubsystem::OnUserInviteAccepted(bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	if (bWasSuccessful == true)
	{
		UE_LOG(LogTemp, Log, TEXT("Invite accepted. Trying to join session."));

		FString LevelKeyString;
		InviteResult.Session.SessionSettings.Get(FName("LevelKey"), LevelKeyString);

		UPWGameInstance* GameInst = UPWGameInstance::Get(this);
		check(GameInst);

		GameInst->SelectedInGameLevelKey = FName(LevelKeyString);

		TArray<FOnlineSessionSearchResult> InviteResults = TArray<FOnlineSessionSearchResult>();
		InviteResults.Add(InviteResult);
		JoinGameSession(InviteResults);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invite accept failed."));
	}

	OnFindSessionCompleteDelegate.Broadcast(bWasSuccessful);
}
