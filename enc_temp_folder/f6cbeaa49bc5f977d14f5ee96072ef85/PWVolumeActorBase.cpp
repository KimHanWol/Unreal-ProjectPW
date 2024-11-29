// LINK

//Default
#include "PWVolumeActorBase.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Core/PWPlayerState.h"
#include "Data/PWGameStruct.h"

APWVolumeActorBase::APWVolumeActorBase(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	BoundMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	BoundMeshComponent->SetupAttachment(RootComponent);

	BoundMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoundMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); // 적절한 오브젝트 타입
	BoundMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoundMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FVector RelativeLocation = BoundMeshComponent->GetRelativeLocation();
	RelativeLocation.Z = DefaultMeshHeight;
	BoundMeshComponent->SetRelativeLocation(RelativeLocation);
	BoundMeshComponent->SetBoundsScale(3.f);
	BoundMeshComponent->SetRelativeScale3D(DefaultActorScale);
}

void APWVolumeActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(BoundMeshComponent) == true)
	{
		BoundMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &APWVolumeActorBase::OnVolumeBeginOverlap);
	}
}

void APWVolumeActorBase::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (IsValid(BoundMeshComponent) == true)
	{
		BoundMeshComponent->OnComponentBeginOverlap.RemoveAll(this);
	}
}

void APWVolumeActorBase::ExecuteByRound()
{
	if (IsValid(BoundMeshComponent) == false)
	{
		ensure(false);
		return;
	}

	TArray<AActor*> OverlappedActors;
	BoundMeshComponent->GetOverlappingActors(OverlappedActors, APWPlayerCharacter::StaticClass());

	for (AActor* OverlappedActor : OverlappedActors)
	{
		Execute_Internal(OverlappedActor);
	}

	if (bDestroyWhenExecute == true)
	{
		Destroy();
	}
}

void APWVolumeActorBase::OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Execute_Internal(Other);

	if (bDestroyWhenExecute == true)
	{
		Destroy();
	}
}