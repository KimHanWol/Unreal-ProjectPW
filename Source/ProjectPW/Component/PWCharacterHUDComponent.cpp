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

	// 로컬 컨트롤러 전용
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false || LocalPlayerController->IsLocalController() == false)
	{
		SetComponentTickEnabled(false);
		return;
	}

	UCharacterInGameHUD* InGameHUDWidget = Cast<UCharacterInGameHUD>(GetUserWidgetObject());
	if (IsValid(InGameHUDWidget) == true)
	{
		InGameHUDWidget->InitializeHUDWidget(Cast<APWPlayerCharacter>(GetOwner()));
	}

	RequestRedraw();
}

void UPWCharacterHUDComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//For always shown on the screen
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false)
	{
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return;
	}

	bool bNeedToShowHUD = true;
	if (OwnerCharacter->IsDead() == true)
	{
		bNeedToShowHUD = false;
	}
	else if(LocalPlayerController->GetTeamSide() != OwnerCharacter->GetTeamSide())
	{	
		bNeedToShowHUD = false;
	}

	SetVisibility(bNeedToShowHUD);

	if (bNeedToShowHUD == true)
	{
		APawn* LocalPlayerPawn = LocalPlayerController->GetPawn();
		if (IsValid(LocalPlayerPawn) == true && IsValid(GetOwner()) == true)
		{
			const FRotator& LookRotator = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), LocalPlayerPawn->GetActorLocation());
			SetWorldRotation(LookRotator);
		}
	}
}