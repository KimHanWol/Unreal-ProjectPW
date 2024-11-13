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

	static class APWPlayerController* GetLocalPlayerController(const UObject* WorldContextObj);
	static class APWPlayerState* GetLocalPlayerState(const UObject* WorldContextObj);
};
