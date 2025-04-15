// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Subsystems/GameInstanceSubsystem.h"

//Default
#include "PWBGMManageSubsystem.generated.h"

UENUM(BlueprintType)
enum class EBGMType : uint8
{
	None,
	MainMenu,
	InGame,
};

UCLASS()
class PROJECTPW_API UPWBGMManageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	static UPWBGMManageSubsystem* Get(const UObject* WorldContextObj);

	void PlayBGM(EBGMType BGMType);

private:

	UPROPERTY(Transient)
	class UAudioComponent* BGMComponent;

	UPROPERTY(Transient)
	EBGMType CurrentBGMType;
};

