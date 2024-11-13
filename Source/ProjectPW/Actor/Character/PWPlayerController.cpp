// LINK

//Default
#include "PWPlayerController.h"

//Engine
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWPlayerState.h"
#include "UI/MasterWidget.h"
#include "Helper/PWGameplayStatics.h"

void APWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//TODO: Event 시스템 구현해서 이벤트로 날리기
	if (IsValid(MasterWidget))
	{
		MasterWidget->AddToViewport();
	}
}

void APWPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APWPlayerController, TeamSide);
}

void APWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SetIsMyTurn(IsMyTurn);
	}

	PlayerStateLoadedDelegate.Broadcast();
}

void APWPlayerController::SC_ChangeTurn_Implementation(bool bMyTurn)
{
	IsMyTurn = bMyTurn;

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SetIsMyTurn(IsMyTurn);
	}

	TurnChangedDelegate.Broadcast(bMyTurn);
}

void APWPlayerController::SetTeamSide(ETeamSide NewTeamSide)
{
	TeamSide = NewTeamSide;
	OnRep_TeamSide();
}

void APWPlayerController::OnRep_TeamSide()
{
	const FString& TeamSideString = UPWGameplayStatics::ConvertEnumToString(this, TeamSide);

	//Load Characters
	for (TActorIterator<APWPlayerCharacter> It(GetWorld()); It; ++It)
	{
		APWPlayerCharacter* PlayerCharacter = *It;
		if (IsValid(PlayerCharacter) == true)
		{
			if (PlayerCharacter->GetTeamSide() == TeamSide)
			{
				CharacterList.Add(PlayerCharacter);
				UE_LOG(LogTemp, Log, TEXT("%s Team Character Loaded : %s"), *TeamSideString, *PlayerCharacter->GetName());
			}
		}
	}
}