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

public:
	
	static UPWSteamMatchMakingSubsystem* Get(const UObject* WorldContextObj);

	void CreateGameSession();
	void FindAndJoinGameSession();
	void DestroyGameSession();

private:

	void FindGameSession();
	void JoinGameSession();

   	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);    
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FSessionStateChanged, bool bWasSuccessful)
	FSessionStateChanged OnCreateSessionCompleteDelegate;
	FSessionStateChanged OnFindSessionCompleteDelegate;
	FSessionStateChanged OnJoinSessionCompleteDelegate;
	FSessionStateChanged OnDestroySessionCompleteDelegate;
        
private:

	FDelegateHandle CreateSessionCompleteHandle;
	FDelegateHandle FindSessionCompleteHandle;
	FDelegateHandle JoinSessionCompleteHandle;
	FDelegateHandle DestroySessionCompleteHandle;
	
	IOnlineSessionPtr OnlineSessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

};

