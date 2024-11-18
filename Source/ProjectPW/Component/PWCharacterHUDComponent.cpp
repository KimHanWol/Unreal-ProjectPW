#include "PWCharacterHUDComponent.h"
// LINK

//Default
#include "PWCharacterHUDComponent.h"

//Engine
#include "Kismet/KismetMathLibrary.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "UI/HUD/CharacterInGameHUD.h"
#include "Helper/PWGameplayStatics.h"

void UPWCharacterHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ensure(IsValid(WidgetClass) == true))
	{
		UCharacterInGameHUD* InGameHUDWidget = Cast<UCharacterInGameHUD>(GetUserWidgetObject());
		if (IsValid(InGameHUDWidget) == true)
		{
			InGameHUDWidget->InitializeHUDWidget(Cast<APWPlayerCharacter>(GetOwner()));
		}
	}
}

void UPWCharacterHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//For always shown on the screen
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false)
	{
		return;
	}

	APawn* LocalPlayerPawn = LocalPlayerController->GetPawn();
	if (IsValid(LocalPlayerPawn) == true && IsValid(GetOwner()) == true )
	{
		const FRotator& LookRotator = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), LocalPlayerPawn->GetActorLocation());
		SetWorldRotation(LookRotator);
	}
}
