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
#include "Data/DataAsset/PWGameSetting.h"
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterMovedDelegate.Broadcast(CurrentTurnActivePoint);
	}

	if (CurrentTurnActivePoint <= 0.f)
	{
		CurrentTurnActivePoint = 0.f;
		
		APWPlayerController* OwningPlayerController = Cast<APWPlayerController>(GetOwningController());
		if (IsValid(OwningPlayerController) == true)
		{
			OwningPlayerController->ChangeTurn(false);
		}
	}
}
