// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

//Game

//Default
#include "PWGameplayStatics.generated.h"

 enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API UPWGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()

public:

	static class APWPlayerController* GetLocalPlayerController(const UObject* WorldContextObj);
	static class APWPlayerController* GetOtherPlayerController(class APWPlayerController* CurrentPlayerController);
	static class TArray<class APWPlayerController*> GetAllPlayerController(const UObject* WorldContextObj);
	static class APWPlayerState* GetLocalPlayerState(const UObject* WorldContextObj);

	static ETeamSide GetLocalPlayerTeamSide(const UObject* WorldContextObj);

	//TODO: 로그 남길 때 쓰기
	template <typename EnumType> 
	static FORCEINLINE FString ConvertEnumToString(const UObject* WorldContextObj, const EnumType InValue)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InValue)); 
	}
};