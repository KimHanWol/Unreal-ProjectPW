// LINK

//Default
#include "PWPlayerState.h"

//Engine
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "PWEventManager.h"
#include "Helper/PWGameplayStatics.h"

void APWPlayerState::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerController = Cast<APWPlayerController>(GetOwningController());

	UPWEventManager* PWEventManger = UPWEventManager::Get(this);
	if (IsValid(PWEventManger) == true)
	{
		PWEventManger->CharacterAliveStateChangedDelegate.AddUObject(this, &APWPlayerState::OnCharacterAliveStateChanged);
	}
}

void APWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APWPlayerState, TeamSide);
	DOREPLIFETIME(APWPlayerState, TeamCharacterDataList);
	DOREPLIFETIME(APWPlayerState, CommanderPawn);
}

void APWPlayerState::SS_InitializePlayerState(ETeamSide InTeamSide)
{
	TeamSide = InTeamSide;
	SS_LoadCharacters();
}

void APWPlayerState::SetCommanderPawn(APawn* InCommanderPawn)
{
	CommanderPawn = InCommanderPawn;
}

APawn* APWPlayerState::GetCommanderPawn()
{
	return CommanderPawn;
}

void APWPlayerState::SetCurrentTurnActivePoint(float InCurrentTurnActivePoint)
{
	CurrentTurnActivePoint = InCurrentTurnActivePoint;
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterMovedDelegate.Broadcast(CurrentTurnActivePoint);
	}

}

void APWPlayerState::SS_LoadCharacters()
{
	TArray<FCharacterAliveData> InTeamCharacterDataList;
	for (TActorIterator<APWPlayerCharacter> PlayerCharacter(GetWorld()); PlayerCharacter; ++PlayerCharacter)
	{
		if (IsValid(*PlayerCharacter) == false)
		{
			continue;
		}

		if (PlayerCharacter->GetTeamSide() == TeamSide)
		{
			FCharacterAliveData CharacterData;
			CharacterData.PlayerCharacter = *PlayerCharacter;
			CharacterData.bIsAlive = true;
			InTeamCharacterDataList.Add(CharacterData);
		}
	}

	TeamCharacterDataList = InTeamCharacterDataList;
	OnRep_TeamCharacterDataList();
}

void APWPlayerState::OnCharacterAliveStateChanged(APWPlayerCharacter* TargetCharacter, bool bIsAlive)
{
	if (IsValid(TargetCharacter) == true)
	{
		for (FCharacterAliveData& CharcterData : TeamCharacterDataList)
		{
			if (CharcterData.PlayerCharacter == TargetCharacter)
			{
				CharcterData.bIsAlive = bIsAlive;
				break;
			}
		}
	}
}

void APWPlayerState::OnCharacterMoved(float Distance)
{	
	//로컬
	if (IsValid(GetPlayerController()) == false || GetPlayerController()->IsLocalPlayerController() == false)
	{
		return;
	}

	CurrentTurnActivePoint -= Distance;
	if (CurrentTurnActivePoint <= 0.f && bIsReadyToMove == true)
	{
		CurrentTurnActivePoint = 0.f;
		bIsReadyToMove = false;

		APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
		if (IsValid(LocalPlayerController) == true)
		{
			LocalPlayerController->CS_NextTurn();
		}
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterMovedDelegate.Broadcast(CurrentTurnActivePoint);
	}
}

const FCharacterAliveData APWPlayerState::GetTeamCharacterData(int32 CharacterNum) const
{
	if (TeamCharacterDataList.Num() > CharacterNum - 1)
	{
		return TeamCharacterDataList[CharacterNum - 1];
	}

	return FCharacterAliveData();
}

TArray<APWPlayerCharacter*> APWPlayerState::GetAliveTeamCharacterList() const
{
	TArray<APWPlayerCharacter*> AliveTeamCharacterList;
	for (const FCharacterAliveData& CharcterData : TeamCharacterDataList)
	{
		if (CharcterData.bIsAlive == true)
		{
			AliveTeamCharacterList.Add(CharcterData.PlayerCharacter);
		}
	}

	return AliveTeamCharacterList;
}

TArray<class APWPlayerCharacter*> APWPlayerState::GetTeamCharacterList() const
{
	TArray<APWPlayerCharacter*> TeamCharacterList;
	for (const FCharacterAliveData& CharcterData : TeamCharacterDataList)
	{
		TeamCharacterList.Add(CharcterData.PlayerCharacter);
	}
	return TeamCharacterList;
}

int32 APWPlayerState::GetAliveTeamCharacterNum() const
{
	return GetAliveTeamCharacterList().Num();
}

void APWPlayerState::SetIsMyTurn(bool bInIsMyTurn)
{
	bIsMyTurn = bInIsMyTurn;

	if (bIsMyTurn == true)
	{
		const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
		if (IsValid(PWGameSetting) == true)
		{
			 CurrentTurnActivePoint = PWGameSetting->TurnActivePoint;
		}

		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TeamCharacterMovedDelegate.Broadcast(CurrentTurnActivePoint);
		}

		bIsReadyToMove = true;
	}
}

void APWPlayerState::OnRep_TeamCharacterDataList()
{
	bIsTeamCharacterInitialized = true;
}
