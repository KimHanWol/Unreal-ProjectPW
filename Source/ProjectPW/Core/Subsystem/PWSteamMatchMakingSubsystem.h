// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"

//Game
#include "Subsystems/GameInstanceSubsystem.h"

//Default
#include "PWSteamMatchMakingSubsystem.generated.h"

UCLASS()
class PROJECTPW_API UPWSteamMatchMakingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPWSteamMatchMakingSubsystem();

protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

public:
	
	static UPWSteamMatchMakingSubsystem* Get(const UObject* WorldContextObj);

	void CreateGameSession(FName InSelectedLevelKey);
	void StartGameSession();
	void FindAndJoinGameSession(FName InSelectedLevelKey);
	void StopMatchMaking();
	void LeaveGameSession();

private:

	void ClientTravelToSessionLevel(FName SessionName);

	void FindGameSession(FName InSelectedLevelKey);
	void JoinGameSession(const TArray<FOnlineSessionSearchResult>& SearchResults);
	void DestroyGameSession();

   	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);    
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnUserInviteAccepted(bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FSessionStateChanged, bool bWasSuccessful)
	FSessionStateChanged OnCreateSessionCompleteDelegate;
	FSessionStateChanged OnFindSessionCompleteDelegate;
	FSessionStateChanged OnJoinSessionCompleteDelegate;
	FSessionStateChanged OnDestroySessionCompleteDelegate;
        
private:

	FDelegateHandle CreateSessionCompleteHandle;
	FDelegateHandle StartSessionCompleteHandle;
	FDelegateHandle FindSessionCompleteHandle;
	FDelegateHandle JoinSessionCompleteHandle;
	FDelegateHandle DestroySessionCompleteHandle;

	FDelegateHandle UserInviteAcceptedHandle;

	FTimerHandle JoinSessionTimerHandle;
	
	IOnlineSessionPtr OnlineSessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//TODO: GameSetting 쪽으로 옮기기
	int32 JoinRetryMaxCount = 5;

	int32 JoinRetryCount = 0;
};

