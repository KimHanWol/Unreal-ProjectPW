// LINK

//Default
#include "PWPlayerCharacter.h"

//Engine

//Game
#include "Component/PWEquipmentComponent.h"
#include "Helper/PWGameplayStatics.h"


APWPlayerCharacter::APWPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PWEquipmentComponent = CreateDefaultSubobject<UPWEquipmentComponent>(TEXT("EquipmentComponent"));
}

void APWPlayerCharacter::Execute_Main_Triggered()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute_Main_Triggered();
	}
}

void APWPlayerCharacter::Execute_Main_Completed()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute_Main_Completed();
	}
}

void APWPlayerCharacter::Execute_Sub_Triggered()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute_Sub_Triggered();
	}
}

void APWPlayerCharacter::Execute_Sub_Completed()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute_Sub_Completed();
	}
}
