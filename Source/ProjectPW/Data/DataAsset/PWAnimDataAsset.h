// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

//Game

//Default
#include "PWAnimDataAsset.generated.h"

UENUM()
enum class EAnimMontageType : uint8
{
	None,
	Death,
	Bandage,
	ADS,
	HammerSmash,
	Build,
};

UENUM()
enum class EAnimationType : uint8
{
	None,
	Idle,
	JumpStart,
	Jumping,
	JumpEnd,
};

enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API UPWAnimDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:

	//TODO: PWGameInstance 나 GameData 쪽에서 AsyncLoad 방법 찾아야 함
	void Initialize();

	static const UPWAnimDataAsset* Get(const UObject* WorldContextObj, ECharacterType CharcterType);

	static TSoftObjectPtr<UAnimMontage> GetAnimMontage(const UObject* WorldContextObj, ECharacterType CharcterType, EAnimMontageType AnimMontageType);
	static TSoftObjectPtr<UAnimationAsset> GetAnimation(const UObject* WorldContextObj, ECharacterType CharcterType, EAnimationType AnimationType);
	static TSoftObjectPtr<UBlendSpace> GetBlendSpace(const UObject* WorldContextObj, ECharacterType CharcterType);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAnimMontageType, TSoftObjectPtr<class UAnimMontage>> MontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAnimationType, TSoftObjectPtr<class UAnimationAsset>> AnimationMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UBlendSpace> BlendSpace;
};
