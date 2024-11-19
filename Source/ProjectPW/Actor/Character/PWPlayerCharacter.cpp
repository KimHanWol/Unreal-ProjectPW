// LINK

//Default
#include "PWPlayerCharacter.h"

//Engine
#include "AbilitySystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Component/PWCharacterHUDComponent.h"
#include "Component/PWEquipmentComponent.h"
#include "Core/PWEventManager.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"

APWPlayerCharacter::APWPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PWEquipmentComponent = CreateDefaultSubobject<UPWEquipmentComponent>(TEXT("EquipmentComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemCompnent"));
	CharacterHUDComponent = CreateDefaultSubobject<UPWCharacterHUDComponent>(TEXT("CharacterHUDComponent"));
	CharacterHUDComponent->SetupAttachment(GetRootComponent());

	PWAttributeSet_Attackable = CreateDefaultSubobject<UPWAttributeSet_Attackable>(TEXT("AttributeSet_Attackable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Attackable);

	PWAttributeSet_Damageable = CreateDefaultSubobject<UPWAttributeSet_Damageable>(TEXT("AttributeSet_Damageable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Damageable);
}

void APWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacterDefaultStats();
}

void APWPlayerCharacter::LifeSpanExpired()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->OnDeath();
	}

	Super::LifeSpanExpired();
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

void APWPlayerCharacter::OnFullyDamaged(IPWAttackableInterface* Killer)
{
	if (bIsDead == true)
	{
		return;
	}
	bIsDead = true;

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterDeadDelegate.Broadcast(this);
	}

	//LookAt
	const FRotator& LookRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cast<AActor>(Killer)->GetActorLocation());
	SetActorRotation(LookRotator);

	if (DeathAnimation.IsNull() == false)
	{
		PlayAnimMontage(DeathAnimation.LoadSynchronous());
	}

	SetLifeSpan(3.f);
}

const FPWCharacterDataTableRow* APWPlayerCharacter::GetCharacterData() const
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	return PWGameData->FindTableRow<FPWCharacterDataTableRow>(EDataTableType::Character, CharacterKey);
}

void APWPlayerCharacter::LoadCharacterDefaultStats()
{
	if (const FPWCharacterDataTableRow* PWCharacterDataTableRow = GetCharacterData())
	{
		FString LogString;

		LogString.Append(FString::Printf(TEXT("%s Attribute Initialized with %s data: \n"), *GetName(), *CharacterKey.ToString()));
		if (IsValid(PWAttributeSet_Damageable) == true)
		{
			float Health = PWCharacterDataTableRow->Health;
			PWAttributeSet_Damageable->InitHealth(Health);
			LogString.Append(FString::Printf(TEXT("\tAttributeSet_Damageable Health :%f \n"), Health));
			PWAttributeSet_Damageable->InitMaxHealth(Health);
			LogString.Append(FString::Printf(TEXT("\tAttributeSet_Damageable MaxHealth :%f \n"), Health));
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