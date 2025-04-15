// LINK

//Default
#include "SpawnCharacterInputHandler.h"

//Engine
#include "EnhancedInputComponent.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/Object/PWSpawnAreaActor.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataAsset/PWGameData.h"

void USpawnCharacterInputHandler::SetupKeyBindings(APWPlayerController* InPWPlayerController)
{
	Super::SetupKeyBindings(InPWPlayerController);

	if (IsValid(InPWPlayerController) == false)
	{
		ensure(false);
		return;
	}
	PWPlayerController = InPWPlayerController;

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PWPlayerController->InputComponent);
	if (IsValid(EnhancedInputComponent) == false)
	{
		return;
	}

	EnhancedInputComponent->BindAction(SelectCharacter_1, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::Select_1);
	EnhancedInputComponent->BindAction(SelectCharacter_2, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::Select_2);
	EnhancedInputComponent->BindAction(SelectCharacter_3, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::Select_3);
	EnhancedInputComponent->BindAction(SpawnAction, ETriggerEvent::Triggered, this, &USpawnCharacterInputHandler::TrySpawn);
}

void USpawnCharacterInputHandler::Select_1(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	Select_Internal(0);
}

void USpawnCharacterInputHandler::Select_2(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	Select_Internal(1);
}

void USpawnCharacterInputHandler::Select_3(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	Select_Internal(2);
}

void USpawnCharacterInputHandler::Select_Internal(int32 SelectIndex)
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == false)
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

	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	if (ensure(CharacterDataList.Num() > 0))
	{
		SelectedCharacterType = CharacterDataList[SelectIndex]->CharacterType;
		PWEventManager->CharacterSelectedForSpawnDelegate.Broadcast(SelectedCharacterType);
	}
}

void USpawnCharacterInputHandler::TrySpawn(const struct FInputActionValue& Value)
{
	if (IsInputEnabled() == false)
	{
		return;
	}

	if (IsValid(PWPlayerController) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerState* PWPlayerState = PWPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return;
	}

	// 현재 팀이 정해지지 않은 상태
	if (PWPlayerState->GetTeamSide() == ETeamSide::None)
	{
		return;
	}

    FVector WorldLocation;
	FVector WorldDirection;

	float ScreenX;
	float ScreenY;

    if (PWPlayerController->GetMousePosition(ScreenX, ScreenY))
    {
        PWPlayerController->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection);

        const FVector& StartLocation = WorldLocation;
        const FVector& EndLocation = WorldLocation + (WorldDirection * 10000.0f);

        TArray<FHitResult> HitResultList;
		bool bIsHitSuccess = GetWorld()->LineTraceMultiByChannel(HitResultList, StartLocation, EndLocation, ECC_Visibility);
		
		// 스폰 구역 찾기
		bool bSpawnAreaFound = false;
		if (bIsHitSuccess == true)
		{
			for (const FHitResult& HitResult : HitResultList)
			{
				APWSpawnAreaActor* PWSpawnAreaActor = Cast<APWSpawnAreaActor>(HitResult.GetActor());
				if (IsValid(PWSpawnAreaActor) == true)
				{
					if (PWSpawnAreaActor->GetTeamSide() == PWPlayerState->GetTeamSide())
					{
						bSpawnAreaFound = true;
						break;
					}
				}
			}
		}
		
		// 스폰 구역에서 땅 찾기
        if (bSpawnAreaFound == true)
        {
			FHitResult SpawnHitResult;
			for (const FHitResult& HitResult : HitResultList)
			{	
				APWSpawnAreaActor* PWSpawnAreaActor = Cast<APWSpawnAreaActor>(HitResult.GetActor());
				if (IsValid(PWSpawnAreaActor) == true)
				{
					continue;
				}

				SpawnHitResult = HitResult;
				break;
			}

			APawn* CommanderPawn = PWPlayerState->GetCommanderPawn();
			if (IsValid(CommanderPawn) == true)
			{
				FRotator CommanderPawnRotator = CommanderPawn->GetActorRotation();
				CommanderPawnRotator.Pitch = 0.f;
				FTransform Transform;
				Transform.SetLocation(SpawnHitResult.ImpactPoint);
				Transform.SetRotation(FQuat(CommanderPawnRotator));

				PWPlayerController->CS_SpawnCharacter(SelectedCharacterType, Transform);
			}
        }
    }
}