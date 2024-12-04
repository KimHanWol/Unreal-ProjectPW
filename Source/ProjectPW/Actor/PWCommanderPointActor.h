// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"

//Default
#include "PWCommanderPointActor.generated.h"

enum class ETeamSide : uint8;

UCLASS(Blueprintable)
class PROJECTPW_API APWCommanderPointActor : public AActor
{
	GENERATED_BODY()
		
	APWCommanderPointActor();

public:

	ETeamSide GetTeamSide() const { return TeamSide; }

	FTransform GetSpawnAreaTransform();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamSide TeamSide;

	UPROPERTY(Transient)
	class UStaticMeshComponent* SpawnAreaLocationActor;
};
