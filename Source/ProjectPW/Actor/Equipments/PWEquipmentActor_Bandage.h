// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Actor/Equipments/PWEquipmentActorBase.h"

//Default
#include "PWEquipmentActor_Bandage.generated.h"


UCLASS()
class PROJECTPW_API APWEquipmentActor_Bandage : public APWEquipmentActorBase
{
	GENERATED_BODY()

public:

	//Bandege
	virtual void Execute_Main_Triggered() override;

protected:

	virtual bool IsInteractableActor(AActor* TargetActor) override;
};
