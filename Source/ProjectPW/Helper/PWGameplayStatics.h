// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

//Game

//Default
#include "PWGameplayStatics.generated.h"

 enum class ETeamSide : uint8;
 enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API UPWGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()

public:

	static class APWPlayerController* GetLocalPlayerController(const UObject* WorldContextObj);
	static class APWPlayerController* GetOtherPlayerController(class APWPlayerController* CurrentPlayerController);
	static class TArray<class APWPlayerController*> GetAllPlayerController(const UObject* WorldContextObj);
	static class TArray<class APWPlayerCharacter*> GetAllPlayerCharacter(const UObject* WorldContextObj);
	static class APWPlayerState* GetLocalPlayerState(const UObject* WorldContextObj);

	static ETeamSide GetLocalPlayerTeamSide(const UObject* WorldContextObj);

	static const struct FPWCharacterDataTableRow* FindCharacterData(const UObject* WorldContextObj, const ECharacterType TargetCharacterType);

	template <typename EnumType> 
	static FORCEINLINE FString ConvertEnumToString(const UObject* WorldContextObj, const EnumType InValue)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<int64>(InValue)); 
	}
};