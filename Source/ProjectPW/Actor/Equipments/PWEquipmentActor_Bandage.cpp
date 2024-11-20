// LINK

//Default
#include "PWEquipmentActor_Bandage.h"

//Engine
#include "AbilitySystemComponent.h"
#include "UObject/ScriptInterface.h"


//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"
#include "Interface/PWHealableInterface.h"

void APWEquipmentActor_Bandage::BeginPlay()
{
	Super::BeginPlay();

	TArray<FSoftObjectPath> ItemsToStream;
	if (Montage_Bandage.IsNull() == false)
	{
		ItemsToStream.AddUnique(Montage_Bandage.ToSoftObjectPath());
	}

	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);
}

void APWEquipmentActor_Bandage::Execute_Main_Triggered()
{
	Super::Execute_Main_Triggered();

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		ensure(false);
		return;
	}

	if (Montage_Bandage.IsNull() == false)
	{
		OwnerCharacter->PlayMontage(Montage_Bandage.LoadSynchronous());
	}

	//Hit Test
	bool bHitSuccess = false;
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	FHitResult HitResult;
	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPointLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
	}

	if (bHitSuccess == true)
	{
		//Hit Success
		const FVector& ShotDirection = -ViewPointRotation.Vector();

		IPWHealableInterface* HealableSource = Cast<IPWHealableInterface>(GetOwner());
		IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(HitResult.GetActor());
		if (HealableSource == nullptr || DamageableTarget == nullptr)
		{
			return;
		}

		const UPWAttributeSet_Healable* OwnerAttributeSet_Healable = OwnerCharacter->GetPWAttributeSet_Healable();
		if (IsValid(OwnerAttributeSet_Healable) == true)
		{
			OwnerCharacter->CS_GiveDamage(TScriptInterface<IPWDamageableInterface>(HitResult.GetActor()), -OwnerAttributeSet_Healable->GetHealAmount());
		}
	}
}

void APWEquipmentActor_Bandage::Execute_Main_Completed()
{	
	Super::Execute_Main_Completed();

}