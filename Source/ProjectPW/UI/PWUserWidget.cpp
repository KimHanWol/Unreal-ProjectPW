// LINK

//Default
#include "PWUserWidget.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Helper/PWGameplayStatics.h"

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