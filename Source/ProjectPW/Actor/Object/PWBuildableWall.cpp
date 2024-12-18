// LINK

//Default
#include "PWBuildableWall.h"

//Engine

//Game
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "Data/DataAsset/PWGameSetting.h"

APWBuildableWall::APWBuildableWall()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemCompnent"));
	PWAttributeSet_Damageable = CreateDefaultSubobject<UPWAttributeSet_Damageable>(TEXT("AttributeSet_Damageable"));
	if (IsValid(AbilitySystemComponent) == true)
	{
		AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Damageable);
	}
}

void APWBuildableWall::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacterDefaultStats();
}

UAbilitySystemComponent* APWBuildableWall::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UPWAttributeSet_Damageable* APWBuildableWall::GetPWAttributeSet_Damageable() const
{
	return PWAttributeSet_Damageable;
}

void APWBuildableWall::LoadCharacterDefaultStats()
{
	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return;
	}

	if (ensure(IsValid(PWAttributeSet_Damageable) == true))
	{
		PWAttributeSet_Damageable->SetMaxHealth(PWGameSetting->BuildableWallDefaultHealth);
		PWAttributeSet_Damageable->SetHealth(PWGameSetting->BuildableWallDefaultHealth);
	}
}

void APWBuildableWall::OnFullyDamaged(IPWAttackableInterface* Killer)
{
	SetActorHiddenInGame(true);
}

void APWBuildableWall::OnRevived()
{
	SetActorHiddenInGame(false);
}
