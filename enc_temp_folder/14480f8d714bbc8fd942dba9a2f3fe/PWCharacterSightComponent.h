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

public:
	
	void EnableSpotLight(bool bEnabled);

	class ASpotLight* SpawnCharacterSightSpotLight();
	class ASpotLight* SpawnCharacterSpotLight();

private:

	//for sight
	//UPROPERTY(Transient)
	//class ASpotLight* CharacterSightSpotLight;

	UPROPERTY(EditAnywhere)
	float SightSpotLightIntensity = 10000.f;

	UPROPERTY(EditAnywhere)
	float SightSpotLightAttenuationRadius = 1000.f;

	//for show character
	//UPROPERTY(Transient)
	//class ASpotLight* CharacterSpotLight;

	UPROPERTY(EditAnywhere)
	float CharacterSpotLightHeightOffset = 300.f;

	UPROPERTY(EditAnywhere)
	float CharacterSpotLightIntensity = 10000.f;

	UPROPERTY(EditAnywhere)
	float CharacterSpotLightInnerConeAngle = 20.f;

	UPROPERTY(EditAnywhere)
	float CharacterSpotLightOuterConeAngle = 30.f;
};