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
}

void APWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APWPlayerState, TeamSide);
	DOREPLIFETIME(APWPlayerState, TeamCharacterList);
	DOREPLIFETIME(APWPlayerState, CommanderPawn);
}

void APWPlayerState::SS_InitializePlayerState(ETeamSide InTeamSide)
{
	TeamSide = InTeamSide;
}

void APWPlayerState::SS_AddCharacter(APWPlayerCharacter* NewCharacter)
{
	TeamCharacterList.Add(NewCharacter);
	OnRep_TeamCharacterList();
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
	float MaxTurnActivePoint = 0.f;
	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (ensure(IsValid(PWGameSetting) == true))
	{
		MaxTurnActivePoint = PWGameSetting->TurnActivePoint;
	}

	CurrentTurnActivePoint = FMath::Clamp(InCurrentTurnActivePoint, 0.f, MaxTurnActivePoint);
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterMovedDelegate.Broadcast(CurrentTurnActivePoint);
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
			LocalPlayerController->CS_RequestNextTurn();
		}
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterMovedDelegate.Broadcast(CurrentTurnActivePoint);
	}
}

APWPlayerCharacter* APWPlayerState::GetTeamCharacter(int32 CharacterNum) const
{
	if (TeamCharacterList.Num() > CharacterNum - 1)
	{
		return TeamCharacterList[CharacterNum - 1];
	}

	return nullptr;
}

TArray<class APWPlayerCharacter*> APWPlayerState::GetTeamCharacterList() const
{
	return TeamCharacterList;
}

int32 APWPlayerState::GetAliveTeamCharacterNum() const
{
	TArray<APWPlayerCharacter*> AliveTeamCharacterList;
	for (APWPlayerCharacter* TeamCharcter : TeamCharacterList)
	{
		if (TeamCharcter->IsDead() == false)
		{
			AliveTeamCharacterList.Add(TeamCharcter);
		}
	}

	return AliveTeamCharacterList.Num();
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

void APWPlayerState::OnRep_TeamCharacterList()
{
	if (TeamCharacterList.Num() >= 5)
	{
		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TeamCharacterAllSpawnedDelegate.Broadcast(Cast<APWPlayerController>(GetPlayerController()), TeamCharacterList);
		}
	}
}
