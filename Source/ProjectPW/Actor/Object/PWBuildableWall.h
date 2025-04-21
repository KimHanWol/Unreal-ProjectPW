// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"
#include "Interface/PWDamageableInterface.h"

//Default
#include "PWBuildableWall.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWBuildableWall : public AActor, public IPWDamageableInterface
{
	GENERATED_BODY()

	APWBuildableWall();

protected:

	virtual void BeginPlay() override;

public:

	//IPWDamageableInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual class UPWAttributeSet_Damageable* GetPWAttributeSet_Damageable() const override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	virtual void LoadCharacterDefaultStats() override;

	virtual void OnFullyDamaged(class IPWAttackableInterface* Killer) override;
	virtual void OnRevived() override;
	//~IPWDamageableInterface

	UFUNCTION()
	void OnRep_IsDestroyed();

private:

	UPROPERTY(VisibleAnywhere)
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	class UPWAttributeSet_Damageable* PWAttributeSet_Damageable;

	UPROPERTY(ReplicatedUsing = OnRep_IsDestroyed)
	bool bIsDestroyed = false;

};
