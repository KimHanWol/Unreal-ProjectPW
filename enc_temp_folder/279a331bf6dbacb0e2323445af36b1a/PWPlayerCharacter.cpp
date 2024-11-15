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

void APWPlayerCharacter::Execute_Main()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute_Main();
	}
}

void APWPlayerCharacter::Execute_Sub()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute_Sub();
	}
}
