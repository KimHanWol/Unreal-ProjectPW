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

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void APWEquipmentActorBase::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
	}
}

void APWEquipmentActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTargetOnCrosshair();
}

void APWEquipmentActorBase::InitializeEquipmentActor(AActor* OwnerActor)
{
	SetOwner(OwnerActor);
}

bool APWEquipmentActorBase::EquipmentActorHitTest(float InMaxRange, ECollisionChannel TargetChannel, FHitResult& OutHitResult, FRotator& OutViewPointRotation)
{
	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return false;
	}
		
	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		return false;
	}

	bool bHitSuccess = false;
	FVector ViewPointLocation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, OutViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + OutViewPointRotation.Vector() * InMaxRange;

	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(OutHitResult, ViewPointLocation, EndLocation, TargetChannel, Params);
	}

	return bHitSuccess;
}

void APWEquipmentActorBase::CheckTargetOnCrosshair()
{
	FHitResult HitResult;
	FRotator ViewPointRotation;
	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_GameTraceChannel1, HitResult, ViewPointRotation);
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
