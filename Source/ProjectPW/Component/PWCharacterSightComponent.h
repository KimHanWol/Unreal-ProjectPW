// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Components/SceneComponent.h"

//Default
#include "PWCharacterSightComponent.generated.h"

UCLASS()
class PROJECTPW_API UPWCharacterSightComponent: public USceneComponent
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:
	
	void EnableSpotLight(bool bEnabled);


private:

	void OnCharacterPossessed(APawn* PossessedPawn, bool bIsCommander);

	void TrySpawnSpotLight();
	void InvalidateSight();

private:

	UPROPERTY(Transient)
	bool bSpotLightEnabled = false;

	UPROPERTY(Transient)
	bool bCharacterPossessed = false;

	//캐릭터 시야 스포트라이트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpotLight> CharacterSightSpotLightClass;

	//머리 위 스포트라이트
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASpotLight> CharacterSpotLightClass;

	UPROPERTY(Transient)
	class ASpotLight* CharacterSightSpotLight;

	UPROPERTY(Transient)
	class ASpotLight* CharacterSpotLight;

	UPROPERTY(EditAnywhere)
	float CharacterSpotLightHeightOffset = 300.f;

};