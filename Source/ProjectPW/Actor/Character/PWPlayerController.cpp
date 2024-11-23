// LINK

//Default
#include "PWPlayerController.h"

//Engine
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Component/PWPlayerInputComponent.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/PWGameEnum.h"
#include "UI/MasterWidget.h"
#include "Helper/PWGameplayStatics.h"

APWPlayerController::APWPlayerController()
{
	bShowMouseCursor = true;

	PlayerInputComponent = CreateDefaultSubobject<UPWPlayerInputComponent>(TEXT("PlayerInputComponent"));
}

void APWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Show widget
	if (IsLocalPlayerController() == true)
	{
		const UPWGameData* PWGameData = UPWGameData::Get(this);
		if (IsValid(PWGameData) == true)
		{
			if (IsValid(MasterWidget) == true)
			{
				MasterWidget->AddToViewport();
			}
		}
	}

	if (IsValid(PlayerInputComponent) == true)
	{
		PlayerInputComponent->InitMappingContext();
	}
}

void APWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return;
	}

	//First possess
	APawn* CommanderPawn = PWPlayerState->GetCommanderPawn();
	if (IsValid(PWPlayerState->GetCommanderPawn()) == false && IsValid(InPawn) == true)
	{
		CommanderPawn = InPawn;
		PWPlayerState->SetCommanderPawn(CommanderPawn);
		InitialPossessedDelegate.Broadcast(this);
	}

	bool bIsCommander = IsValid(CommanderPawn) == true ? CommanderPawn == InPawn : false;
	SC_OnPossess(InPawn, bIsCommander);
;}

void APWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdateTurnData();
}

void APWPlayerController::ChangeTurn(bool bMyTurn)
{
 	if (HasAuthority() == true)
	{
		SC_ChangeTurn(bMyTurn);
		SS_ChangeTurn(bMyTurn);

		APWPlayerController* OtherPlayerController = UPWGameplayStatics::GetOtherPlayerController(this);
		if (IsValid(OtherPlayerController) == true)
		{
			OtherPlayerController->SC_ChangeTurn(!bMyTurn);
			OtherPlayerController->SS_ChangeTurn(!bMyTurn);
		}
	}
	else
	{
		CS_ChangeTurn(bMyTurn);
	}
}

ETeamSide APWPlayerController::GetTeamSide() const
{
	ETeamSide TeamSide = ETeamSide::Red;
	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (ensure(IsValid(PWPlayerState) == true))
	{
		TeamSide = PWPlayerState->GetTeamSide();
	}

	return TeamSide;
}

void APWPlayerController::SC_ChangeTurn_Implementation(bool bMyTurn)
{
	bIsMyTurn = bMyTurn;
	bTurnDataDirty = true;

	UpdateTurnData();
}

void APWPlayerController::CS_ChangeTurn_Implementation(bool bMyTurn)
{
	ChangeTurn(bMyTurn);
}

void APWPlayerController::SC_GameOver_Implementation(bool bWon)
{
	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->GameOverDelegate.Broadcast(bWon);
	}
}

void APWPlayerController::SM_OnHealthChanged_Implementation(AActor* TargetActor, float MaxHealth, float CurrentHealth)
{
	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == false)
	{
		ensure(false);
		return;
	}

	EventManager->HealthChangedDelegate.Broadcast(TargetActor, MaxHealth, CurrentHealth);
}

void APWPlayerController::SelectCharacter(int32 SelectNum)
{
	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return;
	}

	if(HasAuthority() == true)
	{ 
		bool bIsCommander = SelectNum == 0;
		if (bIsCommander == true)
		{
			Possess(PWPlayerState->GetCommanderPawn());
			SC_ChangeInputEnabled(true, false);
		}
		else
		{
			const FCharacterAliveData& CharacterData = PWPlayerState->GetTeamCharacterData(SelectNum);
			if (CharacterData.bIsAlive == true)
			{
				Possess(CharacterData.PlayerCharacter);
				SC_ChangeInputEnabled(false, true);
			}
		}
	}
	else
	{
		CS_SelectCharacter(SelectNum);
	}
}

void APWPlayerController::SS_ChangeTurn(bool bMyTurn)
{
	SelectCharacter(0);
	if(bMyTurn == false)
	{
		SC_ChangeInputEnabled(false, false);
	}
}

void APWPlayerController::SC_ChangeInputEnabled_Implementation(bool bEnableCommander, bool bEnableCharacter)
{
	if (IsValid(PlayerInputComponent) == true)
	{
		PlayerInputComponent->SetInputEnabled(bEnableCommander, bEnableCharacter);
	}
}

void APWPlayerController::SC_OnPossess_Implementation(APawn* PossessedPawn, bool bIsCommander)
{
	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.Broadcast(PossessedPawn, bIsCommander);
	}
}

void APWPlayerController::CS_SelectCharacter_Implementation(int32 SelectNum)
{
	SelectCharacter(SelectNum);
}

void APWPlayerController::UpdateTurnData()
{
	if (bTurnDataDirty == false)
	{
		return;
	}

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SetIsMyTurn(bIsMyTurn);
		TurnChangedDelegate.Broadcast(bIsMyTurn);
		bTurnDataDirty = false;
	}
}
