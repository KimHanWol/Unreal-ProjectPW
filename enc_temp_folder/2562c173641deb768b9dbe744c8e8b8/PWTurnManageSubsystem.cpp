// LINK

//Default
#include "PWTurnManageSubsystem.h"

//Engine
#include "EngineUtils.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/Volume/PWVolumeActorBase.h"
#include "Actor/Volume/PWVolumeActorLocator.h"
#include "Core/PWEventManager.h"
#include "Core/PWGameInstance.h"
#include "Core/PWGameState.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Helper/PWGameplayStatics.h"

UPWTurnManageSubsystem* UPWTurnManageSubsystem::Get(const UObject* WorldContextObj)
{
    UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
    if (IsValid(PWGameInst) == true)
    {
        return PWGameInst->GetPWTurnManageSubsystem();
    }

    return nullptr;
}

void UPWTurnManageSubsystem::StartGame(const TArray<APWPlayerController*>& GamePlayerControllerList)
{
	APWGameState* PWGameState = Cast<APWGameState>(UPWGameplayStatics::GetGameState(this));
	if (IsValid(PWGameState) == true)
	{
		PWGameState->OnStartGame(GamePlayerControllerList.Num());
	}

	TArray<AActor*> VolumeActorLocatorForActorList;
	UPWGameplayStatics::GetAllActorsOfClass(this, APWVolumeActorBase::StaticClass(), VolumeActorLocatorForActorList);
	for (AActor* VolumeActorLocatorForActor : VolumeActorLocatorForActorList)
	{
		APWVolumeActorLocator* PWVolumeActorLocator = Cast<APWVolumeActorLocator>(VolumeActorLocatorForActor);
		if (IsValid(PWVolumeActorLocator) == true)
		{
			VolumeActorLocatorList.Add(PWVolumeActorLocator);
		}
	}

	ValidateTurnData();
    BindEvents();
}

void UPWTurnManageSubsystem::Deinitialize()
{
    Super::Deinitialize();

    UnbindEvents();
}

void UPWTurnManageSubsystem::BindEvents()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->RequestNextTurnDelegate.AddUObject(this, &UPWTurnManageSubsystem::NextTurn);
	}

	UploadSnapshotDataDelegate.AddUObject(this, &UPWTurnManageSubsystem::Snapshot);
}

void UPWTurnManageSubsystem::UnbindEvents()
{
    UPWEventManager* PWEventManager = UPWEventManager::Get(this);
    if (IsValid(PWEventManager) == true)
    {
        PWEventManager->RequestNextTurnDelegate.RemoveAll(this);
    }

	UploadSnapshotDataDelegate.RemoveAll(this);
}

void UPWTurnManageSubsystem::NextTurn()
{
	SnapshotList.Empty();

	ApplyTurnEvent();

	APWGameState* PWGameState = Cast<APWGameState>(UPWGameplayStatics::GetGameState(this));
    if (IsValid(PWGameState) == true)
    {
		PWGameState->NextTurn();
    }

	ValidateTurnData();
}

void UPWTurnManageSubsystem::ValidateTurnData()
{
	APWGameState* PWGameState = Cast<APWGameState>(UPWGameplayStatics::GetGameState(this));
    if (IsValid(PWGameState) == false)
    {
		ensure(false);
		return;
    }

	const TArray<APWPlayerController*>& GamePlayerControllerList =  UPWGameplayStatics::GetAllPlayerController(this);
    for (int32 i = 0; i < GamePlayerControllerList.Num(); i++)
	{
		APWPlayerController* PlayerController = GamePlayerControllerList[i];
		if (IsValid(PlayerController) == true)
		{
			bool bMyTurn = i == PWGameState->GetCurrentPlayerTurn(); //0번 부터 시작
			PlayerController->SC_TurnChanged(bMyTurn);  
		}
	}
}

void UPWTurnManageSubsystem::ApplyTurnEvent()
{
	APWVolumeActorLocator* CandidateVolumeActorLocator = FindGroundForSpawnVolumeActor();
	if (IsValid(CandidateVolumeActorLocator) == false)
	{
		return;
	}

	TSubclassOf<APWVolumeActorBase> TargetVolumeActorClass = UPWGameData::GetVolumeActorRandom(this);
	if (IsValid(TargetVolumeActorClass) == false)
	{
		ensure(false);
		return;
	}

	float SpawnVolumeChance;
	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == true)
	{
		SpawnVolumeChance = PWGameSetting->TurnEvent_SpawnVolumeChance;
	}

	float ChangeFloat = FMath::RandRange(0.f, 1.f);
	if (ChangeFloat < SpawnVolumeChance)
	{
		APWVolumeActorBase* SpawnedVolumeActor = GetWorld()->SpawnActor<APWVolumeActorBase>(TargetVolumeActorClass);
		SpawnedVolumeActor->SetActorLocation(CandidateVolumeActorLocator->GetActorLocation());
		CandidateVolumeActorLocator->SetIsOccupied(true);
	}
}

void UPWTurnManageSubsystem::Snapshot(APawn* PossessedPawn, float CurrentTurnActivePoint)
{
	FString LogString;
	LogString += TEXT("=============================================SNAPSHOT=============================================\n");

	LogString += TEXT("\nSNAPSHOT CHARACTER DATA\n");
	FSnapshotData SnapshotData;
	const TArray<APWPlayerCharacter*>& PlayerCharacterList =  UPWGameplayStatics::GetAllPlayerCharacter(this);
	for (APWPlayerCharacter* PlayerCharacter : PlayerCharacterList)
	{
		FSnapshotCharacterData SnapshotCharacterData;
		SnapshotCharacterData.TargetPlayerCharacter = PlayerCharacter;
		LogString += TEXT("Character : ") + SnapshotCharacterData.TargetPlayerCharacter->GetName() + TEXT("\n");

		SnapshotCharacterData.CharacterTransform = PlayerCharacter->GetActorTransform();
		LogString += TEXT("Transform : ") + SnapshotCharacterData.CharacterTransform.ToString() + TEXT("\n");
		
		const UPWAttributeSet_Damageable* PlayerAttribute_Damageable = PlayerCharacter->GetPWAttributeSet_Damageable();
		if (IsValid(PlayerAttribute_Damageable) == true)
		{
			SnapshotCharacterData.CharacterHealth = PlayerAttribute_Damageable->GetHealth();
			LogString += TEXT("Health : ") + FString::FromInt(SnapshotCharacterData.CharacterHealth) + TEXT("\n");
		}

		SnapshotCharacterData.bIsDead = PlayerCharacter->IsDead();

		SnapshotData.CharacterDataList.Add(SnapshotCharacterData);
	}

	LogString += TEXT("\nSNAPSHOT VOLUME ACTOR DATA\n");
	TArray<AActor*> PWVolumeActorForActorList;
	UPWGameplayStatics::GetAllActorsOfClass(this, APWVolumeActorBase::StaticClass(), PWVolumeActorForActorList);
	for (AActor* PWVolumeActorForActor : PWVolumeActorForActorList)
	{
		APWVolumeActorBase* PWVolumeActor = Cast<APWVolumeActorBase>(PWVolumeActorForActor);
		if (IsValid(PWVolumeActor) == true)
		{
			FSnapshotVolumeActorData SnapshotVolumeActorData;
			SnapshotVolumeActorData.TargetVolumeClass = PWVolumeActor->GetClass();
			LogString += TEXT("Name : ") + SnapshotVolumeActorData.TargetVolumeClass->GetName() + TEXT("\n");

			SnapshotVolumeActorData.VolumeActorTransform = PWVolumeActor->GetActorTransform();
			LogString += TEXT("Transform : ") + SnapshotVolumeActorData.VolumeActorTransform.ToString() + TEXT("\n");
			SnapshotData.VolumeActorDataList.Add(SnapshotVolumeActorData);
		}
	}

	LogString += TEXT("\nSNAPSHOT VOLUME ACTOR LOCATOR DATA\n");
	for (APWVolumeActorLocator* PWVolumeActorLocator : VolumeActorLocatorList)
	{
		if (IsValid(PWVolumeActorLocator) == true)
		{
			SnapshotData.VolumeActorLocatorDataMap.Add(TTuple<APWVolumeActorLocator*, bool>(PWVolumeActorLocator, PWVolumeActorLocator->IsOccupied()));
			LogString += TEXT("Name : ") + PWVolumeActorLocator->GetName() + TEXT("\n");

			FString OccupiedString = PWVolumeActorLocator->IsOccupied() == true ? TEXT("true") : TEXT("false");
			LogString += TEXT("Occupied : ") + OccupiedString + TEXT("\n");
		}
	}

	LogString += TEXT("\nSNAPSHOT STATE DATA\n");
	SnapshotData.TurnActivePoint = CurrentTurnActivePoint;
	LogString += TEXT("TurnActivePoint : ") + FString::FromInt(SnapshotData.TurnActivePoint) + TEXT("\n");
	APWPlayerCharacter* PossassedCharacter = Cast<APWPlayerCharacter>(PossessedPawn);
	if (ensure(IsValid(PossassedCharacter) == true))
	{
		SnapshotData.PossessedCharacter = PossassedCharacter;
		LogString += TEXT("Possessed Character : ") + SnapshotData.PossessedCharacter->GetName() + TEXT("\n");
	}

	UE_LOG(LogTemp, Verbose, TEXT("%s"), *LogString);

	SnapshotList.Push(SnapshotData);
}

APWVolumeActorLocator* UPWTurnManageSubsystem::FindGroundForSpawnVolumeActor()
{
	TArray<AActor*> VolumeActorLocatorActorList;
	UPWGameplayStatics::GetAllActorsOfClass(this, APWVolumeActorLocator::StaticClass(), VolumeActorLocatorActorList);

	TArray<APWVolumeActorLocator*> CandidateLocatorList;
	for (AActor* VolumeActorLocatorActor : VolumeActorLocatorActorList)
	{
		APWVolumeActorLocator* PWVolumeActorLocator = Cast<APWVolumeActorLocator>(VolumeActorLocatorActor);
		if (IsValid(PWVolumeActorLocator) == true)
		{
			if (PWVolumeActorLocator->IsSpawnable() == false)
			{
				CandidateLocatorList.Add(PWVolumeActorLocator);
			}
		}
	}

	if (CandidateLocatorList.Num() <= 0)
	{
		return nullptr;
	}

	int32 CandidateLocatorIndex = FMath::RandRange(0, CandidateLocatorList.Num() - 1);
	return CandidateLocatorList[CandidateLocatorIndex];
}

void UPWTurnManageSubsystem::ApplyPrevSnapshot(APWPlayerController* PlayerControllerInTurn)
{
	if (SnapshotList.Num() <= 0)
	{
		ensure(false);
		return;
	}

	const FSnapshotData& SnapshotData = SnapshotList.Pop();

	FString LogString;
	LogString += TEXT("======================================APPLY SNAPSHOT======================================\n");

	LogString += TEXT("\nAPPLY CHARACTER DATA\n");
	for (const FSnapshotCharacterData& SnapshotCharacterData : SnapshotData.CharacterDataList)
	{
		APWPlayerCharacter* TargetCharacter = SnapshotCharacterData.TargetPlayerCharacter;
		if (IsValid(TargetCharacter) == true)
		{
			LogString += TEXT("Character : ") + TargetCharacter->GetName() + TEXT("\n");

			TargetCharacter->SM_ApplySnapshotTransform(SnapshotCharacterData.CharacterTransform);
			LogString += TEXT("Transform : ") + SnapshotCharacterData.CharacterTransform.ToString() + TEXT("\n");

			UPWAttributeSet_Damageable* PlayerAttribute_Damageable = TargetCharacter->GetPWAttributeSet_Damageable();
			if (IsValid(PlayerAttribute_Damageable) == true)
			{
				PlayerAttribute_Damageable->SetHealth(SnapshotCharacterData.CharacterHealth);
				LogString += TEXT("Health : ") + FString::FromInt(SnapshotCharacterData.CharacterHealth) + TEXT("\n");
			}

			if (TargetCharacter->IsDead() != SnapshotCharacterData.bIsDead)
			{
				TargetCharacter->OnPlayerRevived();
			}
		}
	}

	LogString += TEXT("\nAPPLY VOLUME ACTOR DATA\n");
	TArray<AActor*> VolumeActorForActorList;
	UPWGameplayStatics::GetAllActorsOfClass(this, APWVolumeActorBase::StaticClass(), VolumeActorForActorList);
	for (AActor* VolumeActorForActor : VolumeActorForActorList)
	{
		VolumeActorForActor->Destroy();
	}
	VolumeActorForActorList.Empty();

	for (const FSnapshotVolumeActorData& SnapshotVolumeActorData : SnapshotData.VolumeActorDataList)
	{
		if (IsValid(SnapshotVolumeActorData.TargetVolumeClass) == false)
		{
			ensure(false);
			continue;
		}

		APWVolumeActorBase* SpawnedVolumeActor = GetWorld()->SpawnActor<APWVolumeActorBase>(SnapshotVolumeActorData.TargetVolumeClass);
		if (IsValid(SpawnedVolumeActor) == true)
		{
			SpawnedVolumeActor->SetActorTransform(SnapshotVolumeActorData.VolumeActorTransform);
			LogString += TEXT("Name : ") + SnapshotVolumeActorData.TargetVolumeClass->GetName() + TEXT("\n");
			LogString += TEXT("Transform : ") + SnapshotVolumeActorData.VolumeActorTransform.ToString() + TEXT("\n");
		}
	}

	LogString += TEXT("\nSNAPSHOT VOLUME ACTOR LOCATOR DATA\n");
	for (APWVolumeActorLocator* PWVolumeActor : VolumeActorLocatorList)
	{
		if (IsValid(PWVolumeActor) == false)
		{
			ensure(false);
			return;
		}

		if (SnapshotData.VolumeActorLocatorDataMap.Contains(PWVolumeActor) == true)
		{
			PWVolumeActor->SetIsOccupied(SnapshotData.VolumeActorLocatorDataMap[PWVolumeActor]);
			LogString += TEXT("Name : ") + PWVolumeActor->GetName() + TEXT("\n");

			FString OccupiedString = SnapshotData.VolumeActorLocatorDataMap[PWVolumeActor] == true ? TEXT("true") : TEXT("false");
			LogString += TEXT("Occupied : ") + OccupiedString + TEXT("\n");
		}
	}

	LogString += TEXT("\nAPPLY STATE DATA\n");
	APWPlayerController* LocalPlayerController = PlayerControllerInTurn;
	if (IsValid(LocalPlayerController) == true)
	{
		LocalPlayerController->SM_ApplyTurnActivePoint(SnapshotData.TurnActivePoint);
		LogString += TEXT("TurnActivePoint : ") + FString::FromInt(SnapshotData.TurnActivePoint) + TEXT("\n");

		LocalPlayerController->Possess(SnapshotData.PossessedCharacter);
		LocalPlayerController->SM_PossessBySnapshot(SnapshotData.PossessedCharacter);
		LogString += TEXT("Possessed Character : ") + SnapshotData.PossessedCharacter->GetName() + TEXT("\n");
	}

	UE_LOG(LogTemp, Verbose, TEXT("%s"), *LogString);

	if (SnapshotList.Num() <= 0)
	{
		SnapshotList.Push(SnapshotData);
	}
}
