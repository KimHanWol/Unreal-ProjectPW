// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Components/WidgetComponent.h"

//Default
#include "PWCharacterHUDComponent.generated.h"


UCLASS()
class PROJECTPW_API UPWCharacterHUDComponent: public UWidgetComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

};