// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"

//Default
#include "PWEquipmentActorBase.generated.h"

enum ECollisionChannel : int;

UCLASS()
class PROJECTPW_API APWEquipmentActorBase: public AActor
{
	GENERATED_BODY()

protected:

	APWEquipmentActorBase(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;

public:

	virtual void Execute_Main_Triggered() {};
	virtual void Execute_Main_Completed() {};

	virtual void Execute_Sub_Triggered() {};
	virtual void Execute_Sub_Completed() {};

	class UMeshComponent* GetMesh() { return MeshComponent; }

protected:

	virtual bool EquipmentActorHitTest(float InMaxRange, ECollisionChannel TargetChannel,  FHitResult& OutHitResult, FRotator& OutViewPointRotation);
	virtual bool IsInteractableActor(AActor* TargetActor) { return true; }

private:
	
	void CheckTargetOnCrosshair();
	void OnPlayerPossesssed(APawn* PossessedPawn, bool bIsCommander);

protected:

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

private:

	UPROPERTY(EditAnywhere)
	class UMeshComponent* MeshComponent;

	//타겟을 겨냥하고 있는가?
	UPROPERTY(Transient)
	bool bIsTargetOn = false;

};
