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
#include "UI/MasterWidget.h"
#include "Helper/PWGameplayStatics.h"

APWPlayerController::APWPlayerController()
{
	bShowMouseCursor = true;
}


void APWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//TODO: Event 시스템 구현해서 이벤트로 날리기
	if (IsValid(MasterWidget))
	{
		MasterWidget->AddToViewport();
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

void APWPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(APWPlayerController, TeamSide, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(APWPlayerController, PossessableCharacterList, COND_None, REPNOTIFY_Always);
}

void APWPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//Commander Pawn
	if (IsValid(CommanderPawn) == false)
	{
		CommanderPawn = InPawn;
	}
;}

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

void APWPlayerController::CS_SetTeamSide_Implementation(ETeamSide NewTeamSide)
{
	TeamSide = NewTeamSide;
	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SetTeamSide(NewTeamSide);
	}

	LoadCharacter();
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

void APWPlayerController::SelectCharacter(int32 SelectNum)
{
	if(HasAuthority() == true)
	{ 
		SC_SelectCharacter(SelectNum);

		bool bIsCommander = SelectNum == 0;
		if (bIsCommander == true)
		{
			Possess(CommanderPawn);
			SC_ChangeInputEnabled(true, false);
		}
		else if (PossessableCharacterList.Num() > SelectNum - 1)
		{
			Possess(PossessableCharacterList[SelectNum - 1]);
			SC_ChangeInputEnabled(false, true);
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

void APWPlayerController::LoadCharacter()
{
	//Load Characters
	const FString& TeamSideString = UPWGameplayStatics::ConvertEnumToString(this, TeamSide);

	for (TActorIterator<APWPlayerCharacter> It(GetWorld()); It; ++It)
	{
		APWPlayerCharacter* PlayerCharacter = *It;
		if (IsValid(PlayerCharacter) == true)
		{
			if (PlayerCharacter->GetTeamSide() == TeamSide)
			{
				PossessableCharacterList.AddUnique(PlayerCharacter);
				UE_LOG(LogTemp, Log, TEXT("%s Team Character Loaded : %s"), *TeamSideString, *PlayerCharacter->GetName());
			}
		}
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

void APWPlayerController::CS_SelectCharacter_Implementation(int32 SelectNum)
{
	SelectCharacter(SelectNum);
}

void APWPlayerController::SC_SelectCharacter_Implementation(int32 SelectNum)
{
	bool bIsCommander = SelectNum == 0;
	if (bIsCommander == true)
	{
		//TODO: Event 로 전환
		if (IsValid(MasterWidget))
		{
			MasterWidget->EnableMainWidget(true);
		}

		SetMouseInputToGameAndUI(true);
	}
	else if (PossessableCharacterList.Num() > SelectNum - 1)
	{
		if (IsValid(MasterWidget))
		{
			MasterWidget->EnableMainWidget(false);
		}

		SetMouseInputToGame();
	}
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
