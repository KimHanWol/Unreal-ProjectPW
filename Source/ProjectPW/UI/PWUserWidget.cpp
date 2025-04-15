// LINK

//Default
#include "PWUserWidget.h"

//Engine

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Data/DataAsset/PWSoundData.h"
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
	const UPWSoundData* SoundData = UPWSoundData::Get(this);
	if (ensure(IsValid(SoundData) == true))
	{
		if (ensure(SoundData->MouseHoverSound.IsNull() == false))
		{
			UGameplayStatics::PlaySound2D(this, SoundData->MouseHoverSound.LoadSynchronous());
		}
	}
}

void UPWUserWidget::BP_PlayClickSound()
{
	const UPWSoundData* SoundData = UPWSoundData::Get(this);
	if (ensure(IsValid(SoundData) == true))
	{
		if (ensure(SoundData->MouseClickSound.IsNull() == false))
		{
			UGameplayStatics::PlaySound2D(this, SoundData->MouseClickSound.LoadSynchronous());
		}
	}
}
