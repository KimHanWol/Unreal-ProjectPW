// LINK

//Default
#include "Actor/Character/PWPlayerController.h"
#include "Helper/PWGameplayStatics.h"
#include "PWUserWidget.h"

//Engine

//Game

void UPWUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindEvents();
}


void UPWUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UnbindEvents();
}

void UPWUserWidget::BindEvents()
{
	APWPlayerController* LocalPlayerController = Cast<APWPlayerController>(UPWGameplayStatics::GetLocalPlayerController(this));
	if (IsValid(LocalPlayerController) == true)
	{
		//LocalPlayerController->PlayerCharacterInitializedDelegate.AddUObject(this, &UPWUserWidget::OnPlayerControllerInitialized);
	}
}

void UPWUserWidget::UnbindEvents()
{
	APWPlayerController* LocalPlayerController = Cast<APWPlayerController>(UPWGameplayStatics::GetLocalPlayerController(this));
	if (IsValid(LocalPlayerController) == true)
	{
		//LocalPlayerController->PlayerCharacterInitializedDelegate.RemoveAll(this);
	}
}

void UPWUserWidget::OnPlayerControllerInitialized()
{
	//플레이어가 초기화된 이후에 위젯 Invalidate
	InvalidateWidget();
}