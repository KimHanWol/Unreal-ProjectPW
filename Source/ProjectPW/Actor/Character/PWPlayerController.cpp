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
	LP_SelectCharacter(0);

	bIsMyTurn = bMyTurn;
	bTurnDataDirty = true; //초기화 시점 문제로 PlayerState 에 반영이 안되는 것 방지
	UpdateTurnData();

	if (bMyTurn == false)
	{
		LP_ChangeInputEnabled(false, false);
	}
}

void APWPlayerController::CS_Possess_Implementation(APawn* PossessablePawn, float CurrentTurnActivePointForSnapshot)
{
	Possess(PossessablePawn);

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	//커맨더로 돌아가는 건 무시
	if (PossessablePawn != PWPlayerState->GetCommanderPawn())
	{
		UPWTurnManageSubsystem* PWTurnManageSubsystem = UPWTurnManageSubsystem::Get(this);
		if (IsValid(PWTurnManageSubsystem) == true)
		{
			PWTurnManageSubsystem->UploadSnapshotDataDelegate.Broadcast(PossessablePawn, CurrentTurnActivePointForSnapshot);
		}
	}
}

void APWPlayerController::SC_GameOver_Implementation(bool bWon)
{
	const UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->GameOverDelegate.Broadcast(bWon);
	}
}

void APWPlayerController::CS_LoadPrevSnapshot_Implementation()
{
	UPWTurnManageSubsystem* PWTurnManageSubsystem = UPWTurnManageSubsystem::Get(this);
	if (IsValid(PWTurnManageSubsystem) == true)
	{
		PWTurnManageSubsystem->ApplyPrevSnapshot(this);
	}
}

void APWPlayerController::SM_ApplyTurnActivePoint_Implementation(float InTurnActivePoint)
{
	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	PWPlayerState->SetCurrentTurnActivePoint(InTurnActivePoint);
}

void APWPlayerController::CS_NextTurn_Implementation()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->NextTurnDelegate.Broadcast();
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

	CS_Possess(PossessablePawn, PWPlayerState->GetCurrentTurnActivePoint());

	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.Broadcast(PossessablePawn, bIsCommander);
	}
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

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
	    PWEventManager->TurnChangedDelegate.Broadcast();
	}
}
