// LINK

//Default
#include "PWEquipmentActorBase.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWEventManager.h"

APWEquipmentActorBase::APWEquipmentActorBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	MeshComponent->SetupAttachment(GetRootComponent());
}

void APWEquipmentActorBase::BeginPlay()
{
	Super::BeginPlay();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &APWEquipmentActorBase::OnPlayerPossesssed);
	}
}

void APWEquipmentActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTargetOnCrosshair();
}

bool APWEquipmentActorBase::EquipmentActorHitTest(FHitResult& OutHitResult, FRotator& OutViewPointRotation)
{
	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return false;
	}
		
	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		ensure(false);
		return false;
	}

	bool bHitSuccess = false;
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

	return bHitSuccess;
}

void APWEquipmentActorBase::CheckTargetOnCrosshair()
{
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(HitResult, ViewPointRotation);
	bool bHitOnDemageableActor = bHitSuccess && IsInteractableActor(HitResult.GetActor());

	if (bHitOnDemageableActor != bIsTargetOn)
	{
		bIsTargetOn = bHitOnDemageableActor;

		const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TargetIsOnCrosshairDelegate.Broadcast(Cast<APWPlayerCharacter>(GetOwner()), bIsTargetOn);
		}
	}
}

void APWEquipmentActorBase::OnPlayerPossesssed(APawn* PossessedPawn, bool bIsCommander)
{
	if (IsValid(PossessedPawn) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
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
