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

	static class APWPlayerCharacter* GetLocalPlayerCharacter(const UObject* WorldContextObj);
	static class APWPlayerController* GetLocalPlayerController(const UObject* WorldContextObj);
	static class APWPlayerController* GetOtherPlayerController(const UObject* WorldContextObj);
	static class APWPlayerState* GetLocalPlayerState(const UObject* WorldContextObj);


	template <typename EnumType> 
	static FORCEINLINE FString ConvertEnumToString(const UObject* WorldContextObj, const EnumType InValue)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InValue)); 
	}

	static void AsyncLoadAsset(const struct FSoftObjectPath& AsyncLoadAssetPathList);
	static void AsyncLoadAsset(const TArray<struct FSoftObjectPath>& AsyncLoadAssetPathList);
};