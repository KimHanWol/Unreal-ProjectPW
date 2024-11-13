// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerStart.h"

//Default
#include "PWPlayerStart.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:

	ETeamSide GetTeamSide() const { return TeamSide; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamSide TeamSide;
};
