// LINK

//Default
#include "PWEquipmentActor_Gun.h"

//Engine
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

//Game
#include "Helper/PWGameplayStatics.h"

void APWEquipmentActor_Gun::BeginPlay()
{
	Super::BeginPlay();

	TArray<FSoftObjectPath> ItemsToStream;
	if (MuzzleEffect.IsNull() == false && ImpactEffect.IsNull() == false && Montage_ADS.IsNull() == false)
	{
		ItemsToStream.AddUnique(MuzzleEffect.ToSoftObjectPath());
		ItemsToStream.AddUnique(ImpactEffect.ToSoftObjectPath());
		ItemsToStream.AddUnique(Montage_ADS.ToSoftObjectPath());
	}

	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);
}

void APWEquipmentActor_Gun::Execute_Main()
{
	Super::Execute_Main();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
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

	//Muzzle Effect
	if (MuzzleEffect.IsNull() == false)
	{
		UPWGameplayStatics::SpawnEmitterAttached(
			MuzzleEffect.LoadSynchronous(),
			GetMesh(),
			TEXT("MuzzleFlashSocket")
		);
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

	if (bHitSuccess == false)
	{
		return;
	}

	//Hit Success
	const FVector& ShotDirection = -ViewPointRotation.Vector();
	if (ImpactEffect.IsNull() == false)
	{
		UPWGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect.LoadSynchronous() ,HitResult.Location, ShotDirection.Rotation());
	}
}

void APWEquipmentActor_Gun::Execute_Sub()
{
	Super::Execute_Sub();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}
	
	if (Montage_ADS.IsNull() == false)
	{
		OwnerCharacter->PlayAnimMontage(Montage_ADS.LoadSynchronous());
	}
}