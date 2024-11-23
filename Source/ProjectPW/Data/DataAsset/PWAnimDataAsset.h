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
};

UCLASS()
class PROJECTPW_API UPWAnimDataAsset: public UDataAsset
{
	GENERATED_BODY()

public:

	static const UPWAnimDataAsset* Get(const UObject* WorldContextObj);

	static TSoftObjectPtr<UAnimMontage> GetAnimMontage(const UObject* WorldContextObj, EAnimMontageType AnimMontageType);

protected:

	static UPWAnimDataAsset* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAnimMontageType, TSoftObjectPtr<class UAnimMontage>> AnimationMap;
};
