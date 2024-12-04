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
	Idle_SpawnPreivew,
};

UCLASS()
class PROJECTPW_API UPWAnimDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:

	static const UPWAnimDataAsset* Get(const UObject* WorldContextObj);

	static TSoftObjectPtr<UAnimMontage> GetAnimMontage(const UObject* WorldContextObj, EAnimMontageType AnimMontageType);
	static TSoftObjectPtr<UAnimationAsset> GetAnimation(const UObject* WorldContextObj, EAnimationType AnimationType);

	void Initialize();

protected:

	static UPWAnimDataAsset* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAnimMontageType, TSoftObjectPtr<class UAnimMontage>> MontageMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAnimationType, TSoftObjectPtr<class UAnimationAsset>> AnimationMap;
};
