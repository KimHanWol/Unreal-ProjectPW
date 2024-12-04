// LINK

//Default
#include "PWSpawnAreaActor.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Helper/PWGameplayStatics.h"

enum class ETeamSide : uint8;

APWSpawnAreaActor::APWSpawnAreaActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void APWSpawnAreaActor::BeginPlay()
{
	Super::BeginPlay();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterAllSpawnedDelegate.AddUObject(this, &APWSpawnAreaActor::OnTeamCharacterAllSpawned);
	}
}

void APWSpawnAreaActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterAllSpawnedDelegate.RemoveAll(this);
	}
}

void APWSpawnAreaActor::OnTeamCharacterAllSpawned(const class APWPlayerController* TargetPlayerController, const TArray<class APWPlayerCharacter*>& TargetCharacterList)
{
	if (IsValid(TargetPlayerController) == false)
	{
		return;
	}

	APWPlayerState* PWPlayerState = TargetPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return;
	}

	if (PWPlayerState->GetTeamSide() == TeamSide)
	{
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}