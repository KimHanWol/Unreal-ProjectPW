// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game

//Default
#include "PWGameEnum.generated.h"

UENUM(BlueprintType)
enum class ETeamSide : uint8
{
	None,
	Red,
	Blue,
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	None,
	Soldier,
	Doctor,
};