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

	//캐릭터 시야 스포트라이트
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpotLight> CharacterSightSpotLightClass;

	//머리 위 스포트라이트
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpotLight> CharacterSpotLightClass;

	UPROPERTY(EditAnywhere)
	float CharacterSpotLightHeightOffset = 300.f;

};