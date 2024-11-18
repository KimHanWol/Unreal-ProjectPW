// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSetBase.h"

//Default
#include "PWAttributeSet_Healable.generated.h"


UCLASS()
class PROJECTPW_API UPWAttributeSet_Healable: public UPWAttributeSetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Heal")
	FGameplayAttributeData HealAmount;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Healable, HealAmount)
};
