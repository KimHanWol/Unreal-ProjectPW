// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Character.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"

//Default
#include "PWPlayerCharacter.generated.h"

 enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWPlayerCharacter : public ACharacter, public IPWAttackableInterface, 
															public IPWDamageableInterface
{
	GENERATED_BODY()

	APWPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void BeginPlay() override;
	virtual void LifeSpanExpired() override;

public:

	//Execute
	void Execute_Main_Triggered();
	void Execute_Main_Completed();

	void Execute_Sub_Triggered();
	void Execute_Sub_Completed();

	ETeamSide GetTeamSide() const { return TeamSide; }
	const FName& GetCharacterKey() const { return CharacterKey; }

	//IPWAttackableInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual class UPWAttributeSet_Attackable* GetPWAttributeSet_Attackable() const override;

	//IPWDamageableInterface
	virtual class UPWAttributeSet_Damageable* GetPWAttributeSet_Damageable() const override;
	virtual void OnFullyDamaged(class IPWAttackableInterface* Killer) override;

	const struct FPWCharacterDataTableRow* GetCharacterData() const;

private:

	void LoadCharacterDefaultStats();

private:

	UPROPERTY(EditInstanceOnly)
	ETeamSide TeamSide;

	//Character Table Key
	UPROPERTY(EditInstanceOnly)
	FName CharacterKey;

	//TODO: 애니메이션이 많아지면 AnimSet 으로 이전
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSoftObjectPtr<class UAnimMontage> DeathAnimation;

	UPROPERTY(EditAnywhere)
	class UPWEquipmentComponent* PWEquipmentComponent;

	UPROPERTY(VisibleAnywhere)
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	class UPWCharacterHUDComponent* CharacterHUDComponent;

	UPROPERTY(Transient)
	class UPWAttributeSet_Attackable* PWAttributeSet_Attackable;

	UPROPERTY(Transient)
	class UPWAttributeSet_Damageable* PWAttributeSet_Damageable;

	UPROPERTY(Transient)
	class UPWAttributeSet_Healable* PWAttributeSet_Healable;

	UPROPERTY(Transient)
	bool bIsDead = false;
};
