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

protected:

	virtual void GetLifetimeReplicatedProps( TArray<class FLifetimeProperty> & OutLifetimeProps) const override;

public:

	virtual void BindAttributeChanged() override;
	virtual void UnbindAttributeChanged() override;

private:

	UFUNCTION() void OnRep_Health();
	UFUNCTION() void OnRep_MaxHealth();

	void OnHealthValueChanged(const struct FOnAttributeChangeData& ChangeData);

public:

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Damageable, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UPWAttributeSet_Damageable, MaxHealth)

};
