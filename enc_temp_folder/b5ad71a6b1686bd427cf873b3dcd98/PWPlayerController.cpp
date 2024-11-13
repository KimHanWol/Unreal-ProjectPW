// LINK

//Default
#include "PWPlayerController.h"

//Engine
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputMappingContext.h"


//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/InputHandler/CharacterInputHandler.h"
#include "Core/InputHandler/CommanderInputHandler.h"
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

	if (HasAuthority() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client"));
	}

	TryCreateInputHandler();

	//Input Component 상속받아서 옮기기

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

	DOREPLIFETIME(APWPlayerController, TeamSide);
}

void APWPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);
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

void APWPlayerController::SelectCharacter(int32 SelectNum)
{
	if (SelectNum == 0)
	{
		Possess(CommanderPawn);
	}
	else if (PossessableCharacterList.Num() > SelectNum)
	{
		Possess(PossessableCharacterList[SelectNum - 1]);
	}
}

void APWPlayerController::SetTeamSide(ETeamSide NewTeamSide)
{
	TeamSide = NewTeamSide;
	OnRep_TeamSide();
}

void APWPlayerController::OnRep_TeamSide()
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
				PossessableCharacterList.Add(PlayerCharacter);
				UE_LOG(LogTemp, Log, TEXT("%s Team Character Loaded : %s"), *TeamSideString, *PlayerCharacter->GetName());
			}
		}
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
			UCommanderInputHandler* NewInputHandler = NewObject<UCommanderInputHandler>(this, CommanderInputHandlerClass);
			NewInputHandler->SetupKeyBindings(this, InputComponent);
			NewInputHandler->SetInputEnabled(true);
			CommanderInputHandler = NewInputHandler;
		}

		if (IsValid(CharacterInputHandlerClass) == true)
		{
			UCharacterInputHandler* NewInputHandler = NewObject<UCharacterInputHandler>(this, CharacterInputHandlerClass);
			NewInputHandler->SetupKeyBindings(this, InputComponent);
			NewInputHandler->SetInputEnabled(false);
			CharacterInputHandler = NewInputHandler;
		}
	}
}
