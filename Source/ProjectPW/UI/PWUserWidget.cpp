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