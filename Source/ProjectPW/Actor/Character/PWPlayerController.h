// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerController.h"

//Default
#include "PWPlayerController.generated.h"

enum class ETeamSide : uint8;
enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API APWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	APWPlayerController();

protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;

public:

	ETeamSide GetTeamSide() const;

	UFUNCTION(Server, Reliable)
	void CS_TeamSideInitialized();
	void CS_TeamSideInitialized_Implementation();

	//Turn changed
	UFUNCTION(Client, Reliable)
	void SC_TurnChanged(bool bMyTurn);
	void SC_TurnChanged_Implementation(bool bMyTurn);

	UFUNCTION(Server, Reliable)
	void CS_RequestNextTurn();
	void CS_RequestNextTurn_Implementation();

	//Game start
	UFUNCTION(NetMulticast, Reliable)
	void SM_GameStart();
	void SM_GameStart_Implementation();

	//Game over
	UFUNCTION(NetMulticast, Reliable)
	void SM_GameOver(class APWPlayerController* TargetPlayerController, bool bLose);
	void SM_GameOver_Implementation(class APWPlayerController* TargetPlayerController, bool bLose);

	//Snapshot
	UFUNCTION(Server, Reliable)
	void CS_LoadPrevSnapshot();
	void CS_LoadPrevSnapshot_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void SM_ApplyTurnActivePoint(float InTurnActivePoint);
	void SM_ApplyTurnActivePoint_Implementation(float InTurnActivePoint);

	UFUNCTION(NetMulticast, Reliable)
	void SM_PossessBySnapshot(APawn* PossessablePawn);
	void SM_PossessBySnapshot_Implementation(APawn* PossessablePawn);

	UFUNCTION(Server, Reliable)
	void CS_SpawnActor(TSubclassOf<AActor> SpawnActorClass, const FVector& Location, const FRotator& Rotation);
	void CS_SpawnActor_Implementation(TSubclassOf<AActor> SpawnActorClass, const FVector& Location, const FRotator& Rotation);

	UFUNCTION(Server, Reliable)
	void CS_SpawnCharacter(ECharacterType SpawnCharacterType, const FTransform& SpawnTransform);
	void CS_SpawnCharacter_Implementation(ECharacterType SpawnCharacterType, FTransform SpawnTransform);

	void OnCharacterSelected(int32 SelectNum);
	void LP_SelectCharacter(int32 SelectNum, bool bIsForReset);
	void LP_OnPossess(APawn* PossessedPawn);

private:

	void OnPlayerCharacterAllSpawned(const APWPlayerController* TargetPlayerController, const TArray<class APWPlayerCharacter*>& TeamCharacterList);

	UFUNCTION(Server, Reliable)
	void CS_Possess(APawn* PossessablePawn, float CurrentTurnActivePointForSnapshot);
	void CS_Possess_Implementation(APawn* PossessablePawn, float CurrentTurnActivePointForSnapshot);

	void LP_ChangeInGameInputEnabled(bool bEnableCommander, bool bEnableCharacter);

	void UpdateTurnData();


	void Tick_ShowSpawnPreviewMesh();
	void TryEnableCharacterSpawn();
	void OnSelectedCharacterTypeChanged(ECharacterType SelectedCharacterType);
	void OnSelectedCharacterTypeChanged(int32 SelectedCharacterIndex);

	void OnSelectedCharacterTypeChanged_Internal(const struct FPWCharacterDataTableRow* CharacterData);

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FInitialPossessed, APWPlayerController* SelfPlayerController)
	FInitialPossessed InitialPossessedDelegate;

private:

	UPROPERTY(EditDefaultsOnly)
	class UMasterWidget* MasterWidget;

	//Component
	UPROPERTY(EditAnywhere)
	class UPWPlayerInputComponent* PlayerInputComponent;

	UPROPERTY(Transient)
	class AActor* SpawnPreviewActor;

	UPROPERTY(Transient)
	class USkeletalMeshComponent* SpawnPreviewComponent;

	UPROPERTY(transient)
	bool bIsMyTurn = false;

	UPROPERTY(transient)
	bool bTurnDataDirty = false;

	//TODO: 스폰 컨트롤러 분리
	//Only for server 
	UPROPERTY(transient)
	bool bIsCharacterSpawning = false;

	//Only for local
	UPROPERTY(transient)
	ECharacterType CurrentSelectedCharacterType;

	FTimerHandle CharacterSpawnTimerHandle;
};
