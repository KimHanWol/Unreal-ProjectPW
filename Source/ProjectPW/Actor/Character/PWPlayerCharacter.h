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

	virtual void PossessedBy(AController* NewController) override;

public:

	//Execute
	void Execute_Main_Triggered();
	void Execute_Main_Completed();

	void Execute_Sub_Triggered();
	void Execute_Sub_Completed();

	ETeamSide GetTeamSide() const { return TeamSide; }
	const FName& GetCharacterName() const { return CharacterType; }

	//IPWAttackableInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual class UPWAttributeSet_Attackable* GetPWAttributeSet_Attackable() const override;

	//IPWDamageableInterface
	virtual class UPWAttributeSet_Damageable* GetPWAttributeSet_Damageable() const override;
	virtual void OnFullyDamaged();

private:

	void LoadCharacterDefaultStats();

private:

	UPROPERTY(EditInstanceOnly)
	ETeamSide TeamSide;

	//Character Table Key
	UPROPERTY(EditInstanceOnly)
	FName CharacterType;

	UPROPERTY(EditAnywhere)
	class UPWEquipmentComponent* PWEquipmentComponent;

	UPROPERTY(VisibleAnywhere)
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(Transient)
	class UPWAttributeSet_Attackable* PWAttributeSet_Attackable;

	UPROPERTY(Transient)
	class UPWAttributeSet_Damageable* PWAttributeSet_Damageable;

	UPROPERTY(Transient)
	class UPWAttributeSet_Healable* PWAttributeSet_Healable;

};
