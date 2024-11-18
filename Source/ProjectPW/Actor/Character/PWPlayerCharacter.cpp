// LINK

//Default
#include "PWPlayerCharacter.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Component/PWEquipmentComponent.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Helper/PWGameplayStatics.h"


APWPlayerCharacter::APWPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PWEquipmentComponent = CreateDefaultSubobject<UPWEquipmentComponent>(TEXT("EquipmentComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemCompnent"));

	PWAttributeSet_Attackable = CreateDefaultSubobject<UPWAttributeSet_Attackable>(TEXT("AttributeSet_Attackable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Attackable);

	PWAttributeSet_Damageable = CreateDefaultSubobject<UPWAttributeSet_Damageable>(TEXT("AttributeSet_Damageable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Damageable);
}

void APWPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	LoadCharacterDefaultStats();
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

UAbilitySystemComponent* APWPlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPWAttributeSet_Attackable* APWPlayerCharacter::GetPWAttributeSet_Attackable() const
{
	return PWAttributeSet_Attackable;
}

UPWAttributeSet_Damageable* APWPlayerCharacter::GetPWAttributeSet_Damageable() const
{
	return PWAttributeSet_Damageable;
}

void APWPlayerCharacter::OnFullyDamaged()
{
	//애니메이션 재생
}

void APWPlayerCharacter::LoadCharacterDefaultStats()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	if (const FPWCharacterDataTableRow* PWCharacterDataTableRow = PWGameData->FindTableRow<FPWCharacterDataTableRow>(EDataTableType::Character, CharacterType))
	{
		FString LogString;

		LogString.Append(FString::Printf(TEXT("%s Attribute Initialized with %s data: \n"), *GetName(), *CharacterType.ToString()));
		if (IsValid(PWAttributeSet_Damageable) == true)
		{
			PWAttributeSet_Damageable->InitHealth(PWCharacterDataTableRow->Health);
			LogString.Append(FString::Printf(TEXT("\tAttributeSet_Damageable Health :%f \n"), PWCharacterDataTableRow->Health));
		}
		if (IsValid(PWAttributeSet_Attackable) == true)
		{
			PWAttributeSet_Attackable->InitDamage(PWCharacterDataTableRow->Damage);
			LogString.Append(FString::Printf(TEXT("\tAttributeSet_Attackable Damage :%f \n"), PWCharacterDataTableRow->Damage));
		}
		if (IsValid(PWAttributeSet_Healable) == true)
		{
			PWAttributeSet_Healable->InitHealAmount(PWCharacterDataTableRow->HealAmount);
			LogString.Append(FString::Printf(TEXT("\tAttributeSet_Healable HealAmount :%f \n"), PWCharacterDataTableRow->HealAmount));
		}

		UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);
	}
	else
	{
		ensureMsgf(false, TEXT("There's no character data on CharacterDataTable. Check character name on PWPlayerCharacter."));
	}
}
