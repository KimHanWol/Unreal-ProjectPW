// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSetBase.h"

//Default
#include "PWAttributeSet_Damageable.generated.h"


UCLASS()
class PROJECTPW_API UPWAttributeSet_Damageable: public UPWAttributeSetBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Damageable, Health)
};
