// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"

//Default
#include "PWSpawnAreaActor.generated.h"

enum class ETeamSide : uint8;

UCLASS(Blueprintable)
class PROJECTPW_API APWSpawnAreaActor : public AActor
{
	GENERATED_BODY()
	
	APWSpawnAreaActor();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	ETeamSide GetTeamSide() const { return TeamSide; }

	void OnTeamCharacterAllSpawned(const class APWPlayerController* TargetPlayerController, const TArray<class APWPlayerCharacter*>& TargetCharacterList);

public:

	FTimerHandle TeamSideInitializeWaitTimerHandle;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamSide TeamSide;

	//UPROPERTY(EditAnywhere)
	//class UStaticMeshComponent* StaticMeshComponent;

};
