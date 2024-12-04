// LINK

//Default
#include "PWPlayerController.h"

//Engine
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Object/PWSpawnAreaActor.h"
#include "Component/PWPlayerInputComponent.h"
#include "Core/InputHandler/SpawnCharacterInputHandler.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Core/Subsystem/PWTurnManageSubsystem.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/PWGameEnum.h"
#include "UI/MasterWidget.h"
#include "Helper/PWGameplayStatics.h"

APWPlayerController::APWPlayerController()
{
	CurrentSelectedCharacterType = ECharacterType::Soldier;
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

void APWPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Tick_ShowSpawnPreviewMesh();
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

	if (IsLocalPlayerController() == true)
	{
		if (IsValid(PlayerInputComponent) == true)
		{
			PlayerInputComponent->SetSpawnCharacterInputEnabled(false);
			USpawnCharacterInputHandler* SpawnCharacterInputHandler = PlayerInputComponent->GetSpawnCharacterInputHandler();
			if (ensure(IsValid(SpawnCharacterInputHandler) == true))
			{
				SpawnCharacterInputHandler->SelectedCharacterTypeChangedDelegate.RemoveAll(this);
			}
		}
	}

	bIsCharacterSpawning = false;
	if (IsValid(SpawnPreviewActor) == true)
	{
		SpawnPreviewActor->Destroy();
		SpawnPreviewActor = nullptr;
		SpawnPreviewComponent = nullptr;
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
	}
}

void APWPlayerController::CS_SpawnCharacter_Implementation(ECharacterType SpawnCharacterType, const FTransform& SpawnTransform)
{
	if (IsValid(GetWorld()) == false)
	{
		ensure(false);
		return;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if(IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* SpawnedCharacter = GetWorld()->SpawnActor<APWPlayerCharacter>(PWGameData->PlayerCharacterClass, SpawnTransform);
	if (ensure(IsValid(SpawnedCharacter) == true))
	{
		SpawnedCharacter->SetReplicates(true);

		APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
		if (IsValid(PWPlayerState) == true)
		{
			SpawnedCharacter->InitializeCharacter(GetTeamSide(), SpawnCharacterType);
			PWPlayerState->SS_AddCharacter(SpawnedCharacter);
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

void APWPlayerController::Tick_ShowSpawnPreviewMesh()
{
	if (bIsCharacterSpawning == false)
	{
		return;
	}

	if (IsLocalPlayerController() == false)
	{	
		return;
	}

	if (IsValid(SpawnPreviewActor) == false)
	{
		return;
	}

	APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	FVector WorldLocation;
	FVector WorldDirection;

	float ScreenX;
	float ScreenY;

	if (GetMousePosition(ScreenX, ScreenY))
	{
	    DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection);

	    const FVector& StartLostion = WorldLocation;
	    const FVector& EndLocation = WorldLocation + (WorldDirection * 10000.0f);

	    FHitResult HitResult;
		bool bIsHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, StartLostion, EndLocation, ECC_Visibility);

		bool bSpawnAreaFound = false;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *HitResult.GetActor()->GetName());
	 	APWSpawnAreaActor* PWSpawnAreaActor = Cast<APWSpawnAreaActor>(HitResult.GetActor());
		if (IsValid(PWSpawnAreaActor) == true)
		{
			if (PWSpawnAreaActor->GetTeamSide() == GetTeamSide())
			{
				bSpawnAreaFound = true;
				UE_LOG(LogTemp, Warning, TEXT("Found"));
			}
		}

	    if (bIsHitSuccess == true && bSpawnAreaFound == true)
	    {
	        FVector NewSpawnLocation = HitResult.ImpactPoint;
			NewSpawnLocation += FVector(0.f, 0.f, 1.f); //Offset
			SpawnPreviewActor->SetActorLocation(NewSpawnLocation);

			UE_LOG(LogTemp, Warning, TEXT("%s"), *NewSpawnLocation.ToString());

			APawn* CommanderPawn = PWPlayerState->GetCommanderPawn();
			if (IsValid(CommanderPawn) == true)
			{
				FRotator CommanderPawnRotator = CommanderPawn->GetActorRotation();

				CommanderPawnRotator.Pitch = 0.f;
				CommanderPawnRotator.Roll = 0.f;
				CommanderPawnRotator.Yaw -= 90.f; // 메시 방향 때문에 일부러 설정
				SpawnPreviewActor->SetActorRotation(CommanderPawnRotator);
			}

			if (IsValid(SpawnPreviewComponent) == true)
			{
				SpawnPreviewComponent->SetVisibility(true);
			}
	    }
		else
		{
			if (IsValid(SpawnPreviewComponent) == true)
			{
				SpawnPreviewComponent->SetVisibility(false);
			}
		}
	}
}

void APWPlayerController::TryEnableCharacterSpawn()
{
	if (bIsCharacterSpawning == true)
	{
		return;
	}

	if(IsValid(PlayerInputComponent) == false)
	{
		ensure(false);
		return;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	const APWPlayerState* PWPlayerState = GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	bIsCharacterSpawning = true;

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterAllSpawnedDelegate.AddUObject(this, &APWPlayerController::OnPlayerCharacterAllSpawned);
	}

	if (IsLocalPlayerController() == true)
	{
		//Activate character spawn input
		PlayerInputComponent->SetSpawnCharacterInputEnabled(true);

		USpawnCharacterInputHandler* SpawnCharacterInputHandler = PlayerInputComponent->GetSpawnCharacterInputHandler();
		if (ensure(IsValid(SpawnCharacterInputHandler) == true))
		{
			SpawnCharacterInputHandler->SelectedCharacterTypeChangedDelegate.AddUObject(this, &APWPlayerController::OnSelectedCharacterTypeChanged);
		}

		//Spawn preview actor
		SpawnPreviewActor = GetWorld()->SpawnActor<AActor>();
		if (IsValid(SpawnPreviewActor) == true)
		{
			UActorComponent* MeshActorComponent = SpawnPreviewActor->AddComponentByClass(USkeletalMeshComponent::StaticClass(), false, FTransform::Identity, false);
			SpawnPreviewComponent = Cast<USkeletalMeshComponent>(MeshActorComponent);
			if (IsValid(SpawnPreviewComponent) == true)
			{
				OnSelectedCharacterTypeChanged(CurrentSelectedCharacterType);
			}
		}
	}
}

void APWPlayerController::OnSelectedCharacterTypeChanged(ECharacterType NewCharacterType)
{
	CurrentSelectedCharacterType = NewCharacterType;

	if (IsValid(SpawnPreviewComponent) == false)
	{
		ensure(false);
		return;
	}

	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == true)
	{
		const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
		for (const FPWCharacterDataTableRow* CharacterData : CharacterDataList)
		{
			if (CharacterData->CharacterType == CurrentSelectedCharacterType)
			{
				if (CharacterData->Mesh.IsNull() == false)
				{
					//Mesh setting
					SpawnPreviewComponent->SetSkeletalMesh(CharacterData->Mesh.LoadSynchronous());

					//Set material
					if (ensure(PWGameData->SpawnPreviewMI.IsNull() == false))
					{
						int32 MaterialNum = SpawnPreviewComponent->GetNumMaterials();
						for (int32 i = 0; i < MaterialNum; i++)
						{
							SpawnPreviewComponent->SetMaterial(i, PWGameData->SpawnPreviewMI.LoadSynchronous());
						}
					}

					SpawnPreviewComponent->PlayAnimation(UPWAnimDataAsset::GetAnimation(this, EAnimationType::Idle_SpawnPreivew).LoadSynchronous(), true);
					SpawnPreviewComponent->SetCastShadow(false);
				}
			}
		}
	}
}
