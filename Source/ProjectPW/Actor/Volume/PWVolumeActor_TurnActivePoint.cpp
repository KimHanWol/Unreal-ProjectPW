// LINK

//Default
#include "PWVolumeActor_TurnActivePoint.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWPlayerState.h"

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
	PWTargetPlayerState->SetCurrentTurnActivePoint(CurrentTurnActivePoint + AddableTurnActivePoint);
}
