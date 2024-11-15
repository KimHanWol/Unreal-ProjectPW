// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Character.h"

//Default
#include "PWPlayerCharacter.generated.h"

 enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	APWPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	void Execute_Main_Triggered();
	void Execute_Main_Completed();

	void Execute_Sub_Triggered();
	void Execute_Sub_Completed();

	ETeamSide GetTeamSide() const { return TeamSide; }

private:

	UPROPERTY(EditInstanceOnly)
	ETeamSide TeamSide;

	UPROPERTY(EditAnywhere)
	class UPWEquipmentComponent* PWEquipmentComponent;
};
