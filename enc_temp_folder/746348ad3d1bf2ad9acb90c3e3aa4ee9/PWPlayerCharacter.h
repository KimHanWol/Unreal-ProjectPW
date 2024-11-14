// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Character.h"

//Default
#include "PWPlayerCharacter.generated.h"

 enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ETeamSide GetTeamSide() const { return TeamSide; }

private:

	UPROPERTY(EditInstanceOnly)
	ETeamSide TeamSide;
};
