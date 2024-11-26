// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Character.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"
#include "Interface/PWHealableInterface.h"

//Default
#include "PWPlayerCharacter.generated.h"

 enum class ETeamSide : uint8;
 enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API APWPlayerCharacter : public ACharacter, public IPWAttackableInterface, 
															public IPWDamageableInterface,
															public IPWHealableInterface
{
	GENERATED_BODY()

	APWPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(class AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	void PlayMontage(TSoftObjectPtr<class UAnimMontage> AnimMontage);
	void StopMontage(TSoftObjectPtr<class UAnimMontage> AnimMontage);

	//Execute
	void Execute_Main_Triggered();
	void Execute_Main_Completed();

	void Execute_Sub_Triggered();
	void Execute_Sub_Completed();


	//IPWAttackableInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual class UPWAttributeSet_Attackable* GetPWAttributeSet_Attackable() const override;

	//IPWDamageableInterface
	virtual class UPWAttributeSet_Damageable* GetPWAttributeSet_Damageable() const override;
	virtual void OnFullyDamaged(class IPWAttackableInterface* Killer) override;

	void OnPlayerRevived();

	//IPWHealableInterface
	virtual class UPWAttributeSet_Healable* GetPWAttributeSet_Healable() const override;

	bool IsDead() const { return bIsDead; }
	ETeamSide GetTeamSide() const { return TeamSide; }
	const struct FPWCharacterDataTableRow* GetCharacterData() const;

	UFUNCTION(Server, Reliable)
	void CS_GiveDamage(const TScriptInterface<class IPWDamageableInterface>& Victim, float Damage);
	void CS_GiveDamage_Implementation(const TScriptInterface<class IPWDamageableInterface>& Victim, float Damage);

	UFUNCTION(NetMulticast, Reliable)
	void SM_ApplySnapshotTransform(const FTransform& NewTransform);
	void SM_ApplySnapshotTransform_Implementation(const FTransform& NewTransform);

private:

	void LoadCharacterDefaultStats();
	void ApplyAttributeData();
	void WithdrawAttributeData();

	void OnLocalCharacterPossessed(APawn* PossessedPawn, bool bIsCommander);

	//Montage
	UFUNCTION(Server, Reliable)
	void CS_PlayMontage(class UAnimMontage* AnimMontage);
	void CS_PlayMontage_Implementation(class UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void SM_PlayMontage(class UAnimMontage* AnimMontage);
	void SM_PlayMontage_Implementation(class UAnimMontage* AnimMontage);

	UFUNCTION(Server, Reliable)
	void CS_StopMontage(class UAnimMontage* AnimMontage);
	void CS_StopMontage_Implementation(class UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void SM_StopMontage(class UAnimMontage* AnimMontage);
	void SM_StopMontage_Implementation(class UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void SM_EnableCharacterAnimation(bool bPossessed);
	void SM_EnableCharacterAnimation_Implementation(bool bPossessed);

	UFUNCTION(NetMulticast, Reliable)
	void SM_HideActorByAliveState(bool bHide);
	void SM_HideActorByAliveState_Implementation(bool bHide);

	//기본적인 움직임이 아닌 캐릭터의 모션을 완전히 멈춤
	void EnableCharacterAnimation(bool bEnabled);

protected:

	UPROPERTY(BlueprintReadOnly, Replicated)
	bool bIsDead = false;

private:

	//Data
	UPROPERTY(EditInstanceOnly)
	ETeamSide TeamSide;

	//Character Table Key
	UPROPERTY(EditInstanceOnly)
	ECharacterType CharacterType;

	//Component
	UPROPERTY(EditAnywhere)
	class UPWEquipmentComponent* PWEquipmentComponent;

	UPROPERTY(VisibleAnywhere)
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	class UPWCharacterHUDComponent* CharacterHUDComponent;

	//AttributeSet
	UPROPERTY(Transient)
	class UPWAttributeSet_Attackable* PWAttributeSet_Attackable;

	UPROPERTY(Transient)
	class UPWAttributeSet_Damageable* PWAttributeSet_Damageable;

	UPROPERTY(Transient)
	class UPWAttributeSet_Healable* PWAttributeSet_Healable;

	//ETC
	UPROPERTY(Transient)
	FVector PrevLocation = FVector::ZeroVector;

	UPROPERTY(Transient)
	bool bIgnoreMoveRecordForNextTick = false;

	UPROPERTY(Transient)
	FRotator LastRotationBeforeDeath = FRotator::ZeroRotator;

	FTimerHandle DeathLifeSpanWaitTimerHandle;
};
