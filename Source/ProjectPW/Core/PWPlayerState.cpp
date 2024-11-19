// LINK

//Default
#include "PWPlayerState.h"

//Engine
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "PWEventManager.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"


void APWPlayerState::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacters();
}

void APWPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APWPlayerState, TeamSide);
}

void APWPlayerState::SetTeamSide(ETeamSide InTeamSide)
{
	TeamSide = InTeamSide;
}

void APWPlayerState::LoadCharacters()
{
	for (TActorIterator<APWPlayerCharacter> PlayerCharacter(GetWorld()); PlayerCharacter; ++PlayerCharacter)
	{
		if (IsValid(*PlayerCharacter) == false)
		{
			continue;
		}

		if (PlayerCharacter->GetTeamSide() == TeamSide)
		{
			CharacterList.AddUnique(*PlayerCharacter);
		}
	}
}

void APWPlayerState::SetIsMyTurn(bool bInIsMyTurn)
{
	bIsMyTurn = bInIsMyTurn;
}