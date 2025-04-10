// LINK

//Default
#include "PWVolumeActor_TurnActivePoint.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWAssetLoadManager.h"
#include "Core/PWPlayerState.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void APWVolumeActor_TurnActivePoint::BeginPlay()
{
	Super::BeginPlay();

	//TODO: 이펙트 테이블로 옮기기
	if (ensure(ApplyNS.IsNull() == false))
	{
		UPWAssetLoadManager::AsyncLoad(this, ApplyNS);
	}
}

void APWVolumeActor_TurnActivePoint::Execute_Internal(AActor* OverlappedActor)
{
	const APWPlayerCharacter* TargetCharacter = Cast<APWPlayerCharacter>(OverlappedActor);
	if (IsValid(TargetCharacter) == false)
	{
		return;
	}

	if (TargetCharacter->IsLocallyControlled() == false)
	{
		return;
	}

	const APWPlayerController* PWTargetPlayerController = Cast<APWPlayerController>(TargetCharacter->GetController());
	if (IsValid(PWTargetPlayerController) == false)
	{
		return;
	}

	APWPlayerState* PWTargetPlayerState = PWTargetPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(PWTargetPlayerState) == false)
	{
		return;
	}

	float CurrentTurnActivePoint = PWTargetPlayerState->GetCurrentTurnActivePoint();
	PWTargetPlayerState->SetCurrentTurnActivePoint(CurrentTurnActivePoint + AddableTurnActivePoint, true);
}
