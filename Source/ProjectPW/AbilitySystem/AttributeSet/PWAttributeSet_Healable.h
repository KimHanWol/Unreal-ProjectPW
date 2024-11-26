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

protected:

	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "Heal", Replicated)
	FGameplayAttributeData HealAmount;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Healable, HealAmount)
};
