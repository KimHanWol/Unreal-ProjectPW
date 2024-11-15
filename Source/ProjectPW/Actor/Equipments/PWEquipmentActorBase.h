// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"

//Default
#include "PWEquipmentActorBase.generated.h"


UCLASS()
class PROJECTPW_API APWEquipmentActorBase: public AActor
{
	GENERATED_BODY()

protected:

	APWEquipmentActorBase(const FObjectInitializer& ObjectInitializer);


public:

	virtual void Execute_Main_Triggered() {};
	virtual void Execute_Main_Completed() {};

	virtual void Execute_Sub_Triggered() {};
	virtual void Execute_Sub_Completed() {};

	class UMeshComponent* GetMesh() { return MeshComponent; }

private:

	UPROPERTY(EditAnywhere)
	class UMeshComponent* MeshComponent;

};
