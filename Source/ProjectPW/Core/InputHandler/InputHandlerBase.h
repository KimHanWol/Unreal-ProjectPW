// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game

//Default
#include "InputHandlerBase.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API UInputHandlerBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void SetupKeyBindings(class APWPlayerController* PWPlayerController, class UInputComponent* InputComponent) {};
	
	void SetInputEnabled(bool bInInputEnabled);
	bool IsInputEnabled() const { return bInputEnabled; }

private:

	UPROPERTY(Transient)
	bool bInputEnabled = false;
};
