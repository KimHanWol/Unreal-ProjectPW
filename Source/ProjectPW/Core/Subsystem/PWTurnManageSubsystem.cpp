// LINK

//Default
#include "PWTurnManageSubsystem.h"

//Engine
#include "EngineUtils.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Core/PWGameInstance.h"
#include "Core/PWGameState.h"
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

void UPWTurnManageSubsystem::StartGame(const TArray<APWPlayerController*>& InGamePlayerControllerList)
{
    GamePlayerControllerList = InGamePlayerControllerList;

    for (int32 i = 0; i < GamePlayerControllerList.Num(); i++)
	{
		APWPlayerController* PlayerController = GamePlayerControllerList[i];
		if (IsValid(PlayerController) == true)
		{
			bool bMyTurn = i == CurrentTurnIndex; //0번 부터 시작
			PlayerController->SC_ChangeTurn(bMyTurn);
		}
	}

    PWGameState = Cast<APWGameState>(UPWGameplayStatics::GetGameState(this));
    if (IsValid(PWGameState) == true)
    {
        PWGameState->OnStartGame(GamePlayerControllerList.Num());
    }

	LoadCharacterData();

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
		PWEventManager->NextTurnDelegate.AddUObject(this, &UPWTurnManageSubsystem::NextTurn);
	}

	UploadSnapshotDataDelegate.AddUObject(this, &UPWTurnManageSubsystem::Snapshot);
}

void UPWTurnManageSubsystem::UnbindEvents()
{
    UPWEventManager* PWEventManager = UPWEventManager::Get(this);
    if (IsValid(PWEventManager) == true)
    {
        PWEventManager->NextTurnDelegate.RemoveAll(this);
    }

	UploadSnapshotDataDelegate.RemoveAll(this);
}

void UPWTurnManageSubsystem::NextTurn()
{
	SnapshotList.Empty();

    if (IsValid(PWGameState) == true)
    {
        PWGameState->NextTurn();
        CurrentTurnIndex = PWGameState->GetCurrentPlayerTurn();
    }

    for (int32 i = 0; i < GamePlayerControllerList.Num(); i++)
	{
		APWPlayerController* PlayerController = GamePlayerControllerList[i];
		if (IsValid(PlayerController) == true)
		{
			bool bMyTurn = i == CurrentTurnIndex; //0번 부터 시작
			PlayerController->SC_ChangeTurn(bMyTurn);  
		}
	}
}

void UPWTurnManageSubsystem::LoadCharacterData()
{
	TArray<AActor*> CharacterActorList;
	UPWGameplayStatics::GetAllActorsOfClass(this, APWPlayerCharacter::StaticClass(), CharacterActorList);

	for (AActor* CharacterActor : CharacterActorList)
	{
		APWPlayerCharacter* PWPlayerCharacter = Cast<APWPlayerCharacter>(CharacterActor);	
		if (IsValid(PWPlayerCharacter) == true)
		{
			PlayerCharacterList.Add(PWPlayerCharacter);
		}
	}
}

void UPWTurnManageSubsystem::Snapshot(APawn* PossessedPawn, float CurrentTurnActivePoint)
{
	FString LogString;
	LogString += TEXT("=============================================SNAPSHOT=============================================\n");

	LogString += TEXT("\nSNAPSHOT CHARACTER DATA\n");
	FSnapshotData SnapshotData;
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

		SnapshotData.CharacterDataList.Add(SnapshotCharacterData);
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

	LogString += TEXT("\nAPPLY STATE DATA\n");
	APWPlayerController* LocalPlayerController = PlayerControllerInTurn;
	if (IsValid(LocalPlayerController) == true)
	{
		LocalPlayerController->SM_ApplyTurnActivePoint(SnapshotData.TurnActivePoint);
		LogString += TEXT("TurnActivePoint : ") + FString::FromInt(SnapshotData.TurnActivePoint) + TEXT("\n");

		LocalPlayerController->Possess(SnapshotData.PossessedCharacter);
		LogString += TEXT("Possessed Character : ") + SnapshotData.PossessedCharacter->GetName() + TEXT("\n");
	}

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
		}
	}

	UE_LOG(LogTemp, Verbose, TEXT("%s"), *LogString);

	if (SnapshotList.Num() <= 0)
	{
		SnapshotList.Push(SnapshotData);
	}
}
