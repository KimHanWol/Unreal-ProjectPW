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

protected:

	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

private:

	UFUNCTION() void OnRep_Damage();

public:

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Attackable, Damage)
};
