// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Components/ActorComponent.h"

//Default
#include "PWEquipmentComponent.generated.h"

enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API UPWEquipmentComponent: public UActorComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	void SpawnEquipmentActor(ECharacterType OwnerCharacterType);
	class APWEquipmentActorBase* GetSpawnedEquipmentActor() const { return SpawnedEquipmentActor; }

	void Execute_Main_Triggered();
	void Execute_Main_Completed();

	void Execute_Sub_Triggered();
	void Execute_Sub_Completed();

	void OnAliveStateChanged(bool bAlive);

	bool IsMainTriggered() const { return bMainTriggered; }
	bool IsSubTriggered() const { return bSubTriggered; }

private:

	UPROPERTY(Transient)
	USkeletalMeshComponent* OwnerSkeletalMesh;

	UPROPERTY(Transient)
	class APWEquipmentActorBase* SpawnedEquipmentActor;

	UPROPERTY(Transient)
	bool bMainTriggered = false;

	UPROPERTY(Transient)
	bool bSubTriggered = false;
};