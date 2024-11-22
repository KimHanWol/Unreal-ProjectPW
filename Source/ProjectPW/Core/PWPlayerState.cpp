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

void APWPlayerState::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerController = Cast<APWPlayerController>(GetOwningController());

	UPWEventManager* PWEventManger = UPWEventManager::Get(this);
	if (IsValid(PWEventManger) == true)
	{
		PWEventManger->CharacterDeadDelegate.AddUObject(this, &APWPlayerState::OnCharacterDead);
	}
}

void APWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APWPlayerState, TeamSide);
	DOREPLIFETIME(APWPlayerState, TeamCharacterDataList);
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

void APWPlayerState::OnCharacterDead(APWPlayerCharacter* DeadCharacter)
{
	if (IsValid(DeadCharacter) == true)
	{
		for (FCharacterAliveData& CharcterData : TeamCharacterDataList)
		{
			if (CharcterData.PlayerCharacter == DeadCharacter)
			{
				CharcterData.bIsAlive = false;
				break;
			}
		}
	}
}

void APWPlayerState::OnCharacterMoved(ETeamSide InTeamSide, float Distance)
{
	if (IsValid(GetOwningController()) == false || GetOwningController()->IsLocalPlayerController() == false)
	{
		return;
	}

	if (InTeamSide != TeamSide)
	{
		return;
	}

	CurrentTurnActivePoint -= Distance;
	if (CurrentTurnActivePoint <= 0.f)
	{
		CurrentTurnActivePoint = 0.f;
		
		if (IsValid(OwningPlayerController) == true)
		{
			OwningPlayerController->ChangeTurn(false);
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
	}
}

void APWPlayerState::OnRep_TeamCharacterDataList()
{
	if (IsValid(GetOwningController()) == false || GetOwningController()->IsLocalPlayerController() == false)
	{
		return;
	}

	UPWEventManager* PWEventManger = UPWEventManager::Get(this);
	if (IsValid(PWEventManger) == true)
	{
		PWEventManger->TeamCharcterLoadedDelegate.Broadcast(TeamSide, GetTeamCharacterList());
	}

	bIsTeamCharacterInitialized = true;
}
