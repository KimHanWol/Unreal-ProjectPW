// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Subsystems/GameInstanceSubsystem.h"

//Default
#include "PWTurnManageSubsystem.generated.h"

enum class ETeamSide : uint8;

USTRUCT()
struct FSnapshotCharacterData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Transient)
	class APWPlayerCharacter* TargetPlayerCharacter;

	UPROPERTY(Transient)
	FTransform CharacterTransform;

	UPROPERTY(Transient)
	float CharacterHealth;

	UPROPERTY(Transient)
	bool bIsDead;
};

USTRUCT()
struct FSnapshotData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(Transient)
	TArray<FSnapshotCharacterData> CharacterDataList;

	UPROPERTY(Transient)
	float TurnActivePoint;

	UPROPERTY(Transient)
	class APWPlayerCharacter* PossessedCharacter;
};



UCLASS()
class PROJECTPW_API UPWTurnManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	static UPWTurnManageSubsystem* Get(const UObject* WorldContextObj);

	void StartGame(const TArray<class APWPlayerController*>& GamePlayerControllerList);
	void ApplyPrevSnapshot(class APWPlayerController* PlayerControllerInTurn);

protected:
	
	virtual void Deinitialize() override;

private:

	void BindEvents();
	void UnbindEvents();

	void NextTurn();
	void ValidateTurnData();

	void Snapshot(APawn* PossessedPawn, float CurrentTurnActivePoint);

public:

	DECLARE_MULTICAST_DELEGATE_TwoParams(FUploadSnapshotData, APawn* PossessedPawn, float TurnActivePoint)
	FUploadSnapshotData UploadSnapshotDataDelegate;

private:

	UPROPERTY(Transient)
	TArray<FSnapshotData> SnapshotList;
};

