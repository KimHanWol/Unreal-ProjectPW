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

		if (IsValid(PlayerInputComponent) == true)
		{
			PlayerInputComponent->InitMappingContext();
		}
	}

	TryEnableCharacterSpawn();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.AddUObject(this, &APWPlayerController::OnCharacterSelected);
	}
}

void APWPlayerController::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.RemoveAll(this);
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
}

void APWPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	TryEnableCharacterSpawn();
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

void APWPlayerController::SS_CheckPlayerCharacterSpawned(AActor* SpawnedActor)
{
	APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(SpawnedActor);
	if (IsValid(PWPlayerCharacter) == false)
	{
		return;
	}

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return;
	}

	PWPlayerCharacter->SetTeamSide(GetTeamSide());
	PWPlayerState->SS_AddCharacter(PWPlayerCharacter);
}

void APWPlayerController::OnPlayerCharacterAllSpawned(const APWPlayerController* TargetPlayerController, const TArray<APWPlayerCharacter*>& TeamCharacterList)
{
	if(TargetPlayerController != this)
	{
		return;
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterAllSpawnedDelegate.RemoveAll(this);
	}

	bIsCharacterSpawning = false;

	if (IsLocalPlayerController() == true)
	{
		if (IsValid(PlayerInputComponent) == true)
		{
			PlayerInputComponent->SetSpawnCharacterInputEnabled(false);
		}
	}
}

void APWPlayerController::SC_TurnChanged_Implementation(bool bMyTurn)
{
	LP_SelectCharacter(0, true);

	bIsMyTurn = bMyTurn;
	bTurnDataDirty = true; //초기화 시점 문제로 PlayerState 에 반영이 안되는 것 방지
	UpdateTurnData();

	if (bMyTurn == false)
	{
		LP_ChangeInGameInputEnabled(false, false);
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

void APWPlayerController::SM_GameStart_Implementation()
{
	if (IsLocalPlayerController() == true)
	{
		const UPWEventManager* EventManager = UPWEventManager::Get(this);
		if (IsValid(EventManager) == true)
		{
			EventManager->ShowWidgetDelegate.Broadcast(EWidgetType::MainWidget);
		}
	}
}

void APWPlayerController::SM_GameOver_Implementation(APWPlayerController* TargetPlayerController, bool bLose)
{
	const UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->GameOverDelegate.Broadcast(TargetPlayerController, bLose);
	}

	if (IsLocalPlayerController() == true)
	{
		LP_SelectCharacter(0, true);
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

void APWPlayerController::CS_RequestNextTurn_Implementation()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->RequestNextTurnDelegate.Broadcast();
	}
}

void APWPlayerController::SM_PossessBySnapshot_Implementation(APawn* PossessablePawn)
{
	if (IsLocalController() == true)
	{
		LP_OnPossess(PossessablePawn);
	}
}

void APWPlayerController::CS_SpawnActor_Implementation(TSubclassOf<AActor> SpawnActorClass, const FVector& Location)
{
	if (IsValid(GetWorld()) == false)
	{
		ensure(false);
		return;
	}

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, FTransform(Location));
	if (ensure(IsValid(SpawnedActor) == true))
	{
		SpawnedActor->SetReplicates(true);

		if (bIsCharacterSpawning == true)
		{
			SS_CheckPlayerCharacterSpawned(SpawnedActor);
		}
	}
}

void APWPlayerController::OnCharacterSelected(int32 SelectNum)
{
	LP_SelectCharacter(SelectNum, false);
}

void APWPlayerController::LP_SelectCharacter(int32 SelectNum, bool bIsForReset)
{
	if (bIsMyTurn == false && bIsForReset == false)
	{
		return;
	}

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	//Possess
	APawn* PossessablePawn = nullptr;
	bool bIsCommander = SelectNum == 0;
	if (bIsCommander == true)
	{
		PossessablePawn = PWPlayerState->GetCommanderPawn();
	}
	else
	{
		APWPlayerCharacter* TeamPlayerCharacter = PWPlayerState->GetTeamCharacter(SelectNum);
		if (IsValid(TeamPlayerCharacter) == true && TeamPlayerCharacter->IsDead() == false)
		{
			PossessablePawn = TeamPlayerCharacter;
		}
	}

	CS_Possess(PossessablePawn, PWPlayerState->GetCurrentTurnActivePoint());
	LP_OnPossess(PossessablePawn);
}

void APWPlayerController::LP_OnPossess(APawn* PossessedPawn)
{
	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	bool bIsCommander = PWPlayerState->GetCommanderPawn() == PossessedPawn;
	if (bIsCommander == true)
	{
		LP_ChangeInGameInputEnabled(true, false);
	}
	else
	{
		LP_ChangeInGameInputEnabled(false, true);
	}

	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.Broadcast(PossessedPawn, bIsCommander);
	}
}

void APWPlayerController::LP_ChangeInGameInputEnabled(bool bEnableCommander, bool bEnableCharacter)
{
	if (IsValid(PlayerInputComponent) == true)
	{
		PlayerInputComponent->SetInGameInputEnabled(bEnableCommander, bEnableCharacter);
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

void APWPlayerController::TryEnableCharacterSpawn()
{
	bIsCharacterSpawning = true;
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterAllSpawnedDelegate.RemoveAll(this);
		PWEventManager->TeamCharacterAllSpawnedDelegate.AddUObject(this, &APWPlayerController::OnPlayerCharacterAllSpawned);
	}

	if (IsLocalPlayerController() == true)
	{
		//Activate character spawn input
		APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
		if (IsValid(PWPlayerState) == true && PWPlayerState->GetAliveTeamCharacterNum() < 5)
		{
			if (IsValid(PlayerInputComponent) == true)
			{
				PlayerInputComponent->SetSpawnCharacterInputEnabled(true);
			}
		}
	}
}
