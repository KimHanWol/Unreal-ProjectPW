// LINK

//Default
#include "PWPlayerCharacter.h"

//Engine

//Game
#include "Component/PWEquipmentComponent.h"


APWPlayerCharacter::APWPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PWEquipmentComponent = CreateDefaultSubobject<UPWEquipmentComponent>(TEXT("EquipmentComponent"));
}

void APWPlayerCharacter::Execute()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->Execute();
	}
}
