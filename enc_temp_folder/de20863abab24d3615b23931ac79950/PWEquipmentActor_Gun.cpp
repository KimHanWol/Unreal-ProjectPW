// LINK

//Default
#include "PWEquipmentActor_Gun.h"

//Engine
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "UObject/ScriptInterface.h" 

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"
#include "Helper/PWGameplayStatics.h"
#include "Interface/PWAttackableInterface.h"
#include "Interface/PWDamageableInterface.h"


APWEquipmentActor_Gun::APWEquipmentActor_Gun(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

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

	// TODO: void AddSoftObjectReferences(std::initializer_list<TSoftObjectPtr<AActor>> References);
	// 가변인자 받을 수 있게 수정해서 더 간편하게 호출하기
	// AssetLoadManager 같은거 만들어서 SoftObjectPath로 Map 만들고 캐시한 데이터 가져올 수 있게 해보자.
	UPWGameplayStatics::AsyncLoadAsset(ItemsToStream);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &APWEquipmentActor_Gun::OnPlayerPossesssed);
	}
}

void APWEquipmentActor_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Cast<APWPlayerCharacter>(GetOwner())->IsPlayerControlled() == false)
	{
		return;
	}

	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EqiupmentActorLineTrace(HitResult, ViewPointRotation);

	//공격할 수 있는 적일 때만
	//TODO: 이것도 조준 가능한 인터페이스로 만들기
	// 어빌리티로 만들어야 하나?
	bool bHitOnDemageableActor = bHitSuccess && (Cast<IPWDamageableInterface>(HitResult.GetActor()) != nullptr);

	UE_LOG(LogTemp, Warning, TEXT("%s"), GetOwner()->HasAuthority() ? *FString("Server") : * FString("Client"));
	UE_LOG(LogTemp, Warning, TEXT("%s"), bHitOnDemageableActor ? *FString("True") : * FString("False"));

	//if (bHitOnDemageableActor != bIsTargetOn)
	{
		bIsTargetOn = bHitOnDemageableActor;

		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TargetIsOnCrosshairDelegate.Broadcast(Cast<APWPlayerCharacter>(GetOwner()), bIsTargetOn);
		}
	}
}

void APWEquipmentActor_Gun::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	SetActorTickEnabled(false);
	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(NewOwner);
	if (IsValid(OwnerCharacter) == true)
	{
		if (OwnerCharacter->HasAuthority() == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("Server"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Client"));
		}

		//if (UPWGameplayStatics::GetLocalPlayerTeamSide(this) == OwnerCharacter->GetTeamSide())
		{
			//서버 쪽에서 NetConnection이 없는 서버 주체 캐릭터만
			if (OwnerCharacter->HasAuthority() == true)
			{
				if (IsValid(OwnerCharacter->GetNetConnection()) == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("ss"));
					SetActorTickEnabled(true);
				}
			}
			//클라 쪽에서 LocallyControlled 되고 있는 클라 주체 캐릭터만
			else 
			//if(OwnerCharacter->IsLocallyControlled() == true)
			{
				UE_LOG(LogTemp, Warning, TEXT("ss"));
				SetActorTickEnabled(true);
			}
		}
	}
}

void APWEquipmentActor_Gun::Execute_Main_Triggered()
{
	Super::Execute_Main_Triggered();

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
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

	EnableADS(true);

	FHitResult HitResult;
	FRotator ViewPointRotation;
	//Hit Test
	bool bHitSuccess = EqiupmentActorLineTrace(HitResult, ViewPointRotation);

	if (bHitSuccess == true)
	{
		//Hit Success
		const FVector& ShotDirection = -ViewPointRotation.Vector();
		if (ImpactEffect.IsNull() == false)
		{
			UPWGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect.LoadSynchronous() ,HitResult.Location, ShotDirection.Rotation());
		}

		IPWAttackableInterface* AttackableSource = Cast<IPWAttackableInterface>(GetOwner());
		IPWDamageableInterface* DamageableTarget = Cast<IPWDamageableInterface>(HitResult.GetActor());
		if (AttackableSource == nullptr || DamageableTarget == nullptr)
		{
			return;
		}

		UPWAttributeSet_Attackable* OwnerAttribute_Attackable = OwnerCharacter->GetPWAttributeSet_Attackable();
		if (IsValid(OwnerAttribute_Attackable) == true)
		{
			OwnerCharacter->CS_GiveDamage(TScriptInterface<IPWDamageableInterface>(HitResult.GetActor()), OwnerAttribute_Attackable->GetDamage());
		}
	}
}

void APWEquipmentActor_Gun::Execute_Main_Completed()
{
	if (bIsSubExecuting == false)
	{
		EnableADS(false);
	}
}

void APWEquipmentActor_Gun::Execute_Sub_Triggered()
{
	Super::Execute_Sub_Triggered();

	bIsSubExecuting = true;
	EnableADS(true);
}

void APWEquipmentActor_Gun::Execute_Sub_Completed()
{
	Super::Execute_Sub_Completed();

	bIsSubExecuting = false;
	EnableADS(false);
}

void APWEquipmentActor_Gun::EnableADS(bool bEnabled)
{
	if (bEnabled == bIsADSAnimPlaying)
	{
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	if (Montage_ADS.IsNull() == false)
	{
		if (bEnabled == true)
		{
			OwnerCharacter->PlayMontage(Montage_ADS);
			bIsADSAnimPlaying = true;
		}
		else
		{
			OwnerCharacter->StopMontage(Montage_ADS);
			bIsADSAnimPlaying = false;
		}
	}
}

bool APWEquipmentActor_Gun::EqiupmentActorLineTrace(FHitResult& OutHitResult, FRotator& OutViewPointRotation)
{
	bool bHitSuccess = false;
	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return bHitSuccess;
	}

	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		return bHitSuccess;
	}

	FVector ViewPointLocation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, OutViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + OutViewPointRotation.Vector() * MaxRange;

	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(OutHitResult, ViewPointLocation, EndLocation, ECollisionChannel::ECC_GameTraceChannel1, Params);
	}

	bool bHitOnDamageableActor = Cast<IPWDamageableInterface>(OutHitResult.GetActor()) != nullptr;

	bHitSuccess &= bHitOnDamageableActor;

	return bHitSuccess;
}

void APWEquipmentActor_Gun::OnPlayerPossesssed(APawn* PossessedPawn, bool bIsCommander)
{
	if (IsValid(PossessedPawn) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	if (PossessedPawn == OwnerCharacter)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}
