// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

//Game

//Default
#include "PWGameStruct.generated.h"

USTRUCT(BlueprintType)
struct FSetByCallerData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "SetByCaller"))
	struct FGameplayTag SetByCallerTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude = 0.0f;
};