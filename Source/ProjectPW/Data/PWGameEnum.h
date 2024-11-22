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
	Red,
	Blue,
};

//TODO: 캐릭터 클래스 추가