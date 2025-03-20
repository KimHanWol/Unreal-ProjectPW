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

	//StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//StaticMeshComponent->SetupAttachment(RootComponent);
}

void APWSpawnAreaActor::BeginPlay()
{
	Super::BeginPlay();

	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false)
	{
		ensure(false);
		return;
	}

	if (LocalPlayerController->IsLocalController() == false)
	{
		return;
	}

	//같은 팀 스폰 위치만 노출
	GetWorldTimerManager().SetTimer(TeamSideInitializeWaitTimerHandle, FTimerDelegate::CreateLambda([WeakThis = TWeakObjectPtr<APWSpawnAreaActor>(this), LocalPlayerController]()
		{
			if (WeakThis.IsValid() == true)
			{
				if (IsValid(LocalPlayerController) == true)
				{
					APWPlayerState* PWPlayerState = LocalPlayerController->GetPlayerState<APWPlayerState>();
					if (IsValid(PWPlayerState) == true && PWPlayerState->IsTeamSideInitialized() == true)
					{
						if (PWPlayerState->GetTeamSide() == WeakThis->GetTeamSide())
						{
							WeakThis->SetActorHiddenInGame(false);
						}
						else
						{
							WeakThis->SetActorHiddenInGame(true);
						}

						WeakThis->GetWorldTimerManager().ClearTimer(WeakThis->TeamSideInitializeWaitTimerHandle);
					}
				}
			}
		}), 0.1f, true);

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