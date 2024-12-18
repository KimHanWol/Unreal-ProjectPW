﻿// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game

//Default
#include "PWHealableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPWHealableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPWHealableInterface
{
	GENERATED_BODY()

public:

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const = 0;
	virtual class UPWAttributeSet_Healable* GetPWAttributeSet_Healable() const = 0;

	virtual void LoadCharacterDefaultStats() = 0;

};
