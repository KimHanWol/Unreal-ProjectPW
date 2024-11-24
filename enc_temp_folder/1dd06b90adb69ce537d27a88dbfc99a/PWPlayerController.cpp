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
#include "Core/Subsystem/PWTurnManageSubsystem.h"
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.AddUObject(this, &APWPlayerController::LP_SelectCharacter);
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
	if (IsValid(CommanderPawn) == false && IsValid(InPawn) == true)
	{
		CommanderPawn = InPawn;
		PWPlayerState->SetCommanderPawn(CommanderPawn);
		InitialPossessedDelegate.Broadcast(this);
	}
;}

void APWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UpdateTurnData();
}

void APWPlayerController::CS_NextTurn_Implementation()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->NextTurnDelegate.Broadcast();
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
	bTurnDataDirty = true; //초기화 시점 문제로 PlayerState 에 반영이 안되는 것 방지
	UpdateTurnData();

	LP_SelectCharacter(0);
	if (bMyTurn == false)
	{
		LP_ChangeInputEnabled(false, false);
	}
}

void APWPlayerController::CS_Possess_Implementation(APawn* PossessablePawn)
{
	Possess(PossessablePawn);
}

void APWPlayerController::SC_GameOver_Implementation(bool bWon)
{
	const UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->GameOverDelegate.Broadcast(bWon);
	}
}

void APWPlayerController::SM_OnHealthChanged_Implementation(AActor* TargetActor, float MaxHealth, float CurrentHealth)
{
	const UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->HealthChangedDelegate.Broadcast(TargetActor, MaxHealth, CurrentHealth);
	}
}

void APWPlayerController::LP_SelectCharacter(int32 SelectNum)
{
	if (bIsMyTurn == false)
	{
		return;
	}

	bool bIsCommander = SelectNum == 0;
	if (bIsCommander == true)
	{
		LP_ChangeInputEnabled(true, false);
	}
	else
	{
		LP_ChangeInputEnabled(false, true);
	}

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	//Possess
	APawn* PossessablePawn = nullptr;
	if (bIsCommander == true)
	{
		PossessablePawn = PWPlayerState->GetCommanderPawn();
	}
	else
	{
		const FCharacterAliveData& CharacterData = PWPlayerState->GetTeamCharacterData(SelectNum);
		if (CharacterData.bIsAlive == true)
		{
			PossessablePawn = CharacterData.PlayerCharacter;
		}
	}

	CS_Possess(PossessablePawn);

	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.Broadcast(PossessablePawn, bIsCommander);
	}
}

void APWPlayerController::SC_ChangeInputEnabled_Implementation(bool bEnableCommander, bool bEnableCharacter)
{
	LP_ChangeInputEnabled(bEnableCommander, bEnableCharacter);
}

void APWPlayerController::LP_ChangeInputEnabled(bool bEnableCommander, bool bEnableCharacter)
{
	if (IsValid(PlayerInputComponent) == true)
	{
		PlayerInputComponent->SetInputEnabled(bEnableCommander, bEnableCharacter);
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
		bTurnDataDirty = false;
	}
}
