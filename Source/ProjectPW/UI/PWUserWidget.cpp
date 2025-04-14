// LINK

//Default
#include "PWUserWidget.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Data/DataAsset/PWGameData.h"
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

void UPWUserWidget::BP_PlayHoverSound()
{
	const UPWGameData* GameData = UPWGameData::Get(this);
	if (ensure(IsValid(GameData) == true))
	{
		if (ensure(GameData->MouseHoverSound.IsNull() == false))
		{
			UGameplayStatics::PlaySound2D(this, GameData->MouseHoverSound.LoadSynchronous());
		}
	}
}

void UPWUserWidget::BP_PlayClickSound()
{
	const UPWGameData* GameData = UPWGameData::Get(this);
	if (ensure(IsValid(GameData) == true))
	{
		if (ensure(GameData->MouseClickSound.IsNull() == false))
		{
			UGameplayStatics::PlaySound2D(this, GameData->MouseClickSound.LoadSynchronous());
		}
	}
}
