// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

//Game

//Default
#include "PWGameplayStatics.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API UPWGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()

public:

	static class APlayerController* GetLocalPlayerController(const UObject* WorldContextObj);
};
