// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSetBase.h"

//Default
#include "PWAttributeSet_Attackable.generated.h"


UCLASS()
class PROJECTPW_API UPWAttributeSet_Attackable: public UPWAttributeSetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Attackable, Damage)
};
