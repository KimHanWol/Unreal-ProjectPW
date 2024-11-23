// LINK

//Default
#include "PWPlayerController.h"

//Engine
#include "EngineUtils.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Net/UnrealNetwork.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/InputHandler/CharacterInputHandler.h"
#include "Core/InputHandler/CommanderInputHandler.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/PWGameEnum.h"
#include "UI/MasterWidget.h"
#include "Helper/PWGameplayStatics.h"

APWPlayerController::APWPlayerController()
{
	bShowMouseCursor = true;
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

	//TODO: PlayerInputComponent 구현해서 넣기?
	TryCreateInputHandler();
	if (IsValid(DefaultContext))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		if (IsValid(EnhancedInputSubsys) == true)
		{
			EnhancedInputSubsys->AddMappingContext(DefaultContext, 0);
		}
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

void APWPlayerController::SetMouseInputToGameAndUI(bool bInShowWithCursor)
{
	bShowMouseCursor = bInShowWithCursor;

	FlushPressedKeys();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void APWPlayerController::SetMouseInputToGame()
{
	bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
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
	TryCreateInputHandler();

	if (IsValid(CommanderInputHandler) == false ||
		IsValid(CharacterInputHandler) == false)
	{
		ensure(false);
		return;
	}

	CommanderInputHandler->SetInputEnabled(bEnableCommander);
	CharacterInputHandler->SetInputEnabled(bEnableCharacter);
}

void APWPlayerController::SC_OnPossess_Implementation(APawn* PossessedPawn, bool bIsCommander)
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == false)
	{
		ensure(false);
		return;
	}

	if (bIsCommander == true)
	{
		SetMouseInputToGameAndUI(true);
	}
	else
	{
		SetMouseInputToGame();
	}

	PWEventManager->PlayerPossessedDelegate.Broadcast(PossessedPawn, bIsCommander);

	//if (IsLocalPlayerController() == true)
	//{
	//	PWEventManager->PlayerPossessedDelegate.Broadcast(PossessedCharacter, bIsCommander);
	//}
}

void APWPlayerController::CS_SelectCharacter_Implementation(int32 SelectNum)
{
	SelectCharacter(SelectNum);
}

void APWPlayerController::TryCreateInputHandler()
{
	if (IsValid(CommanderInputHandler) == true &&
		IsValid(CharacterInputHandler) == true)
	{
		return;
	}

	if (IsValid(InputComponent) == true)
	{
		if (IsValid(CommanderInputHandlerClass) == true)
		{
			CommanderInputHandler = NewObject<UCommanderInputHandler>(this, CommanderInputHandlerClass);
			CommanderInputHandler->SetupKeyBindings(this, InputComponent);
			CommanderInputHandler->SetInputEnabled(false);
		}

		if (IsValid(CharacterInputHandlerClass) == true)
		{
			CharacterInputHandler = NewObject<UCharacterInputHandler>(this, CharacterInputHandlerClass);
			CharacterInputHandler->SetupKeyBindings(this, InputComponent);
			CharacterInputHandler->SetInputEnabled(false);
		}
	}
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
