// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

//Game

//Default
#include "PWSoundData.generated.h"

UCLASS()
class PROJECTPW_API UPWSoundData : public UDataAsset
{
	GENERATED_BODY()

public:

	static const UPWSoundData* Get(const UObject* WorldContextObj);

	void Initialize();

protected:

	static UPWSoundData* Instance;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<class USoundBase> MouseHoverSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<class USoundBase> MouseClickSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<class USoundBase> NotificationWidgetSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> AimDownSightSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> BuildSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> RifleFireSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> FootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> HammerSmash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> PossessSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "InGame")
	TSoftObjectPtr<class USoundBase> InJectSound;
};
