// LINK

//Default
#include "PWPlayerCharacter.h"

//Engine
#include "AbilitySystemComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

//Game
#include "AbilitySystem/AttributeSet/PWAttributeSet_Attackable.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Damageable.h"
#include "AbilitySystem/AttributeSet/PWAttributeSet_Healable.h"
#include "Component/PWCharacterHUDComponent.h"
#include "Component/PWEquipmentComponent.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Helper/PWGameplayStatics.h"

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

	PWAttributeSet_Healable = CreateDefaultSubobject<UPWAttributeSet_Healable>(TEXT("AttributeSet_Healable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Healable);
}

void APWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacterDefaultStats();
}

void APWPlayerCharacter::LifeSpanExpired()
{
	if (HasAuthority() == true)
	{
		SM_OnLifeSpanExpired();
	}

	Super::LifeSpanExpired();
}

void APWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PrevLocation == FVector::ZeroVector)
	{
		PrevLocation = GetActorLocation();
		return;
	}

    FVector CurrentLocation = GetActorLocation();
    float DistanceMoved = FVector::Dist(CurrentLocation, PrevLocation);
    PrevLocation = CurrentLocation;

	if (DistanceMoved > 0.f)
	{
		APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
		if (IsValid(PWPlayerState) == true)
		{
			PWPlayerState->OnCharacterMoved(GetTeamSide(), DistanceMoved / 100.f);
		}
	}
}

void APWPlayerCharacter::PossessedBy(class AController* NewController)
{
	Super::PossessedBy(NewController);
	SM_EnableCharacterAnimation(true);

	if (IsLocallyControlled() == true)
    {
        SetActorTickEnabled(true);
	}
}

void APWPlayerCharacter::UnPossessed()
{
	PrevLocation = FVector::ZeroVector;

	SM_EnableCharacterAnimation(false);

	if (IsLocallyControlled() == true)
    {
        SetActorTickEnabled(false);
	}

	Super::UnPossessed();
}

void APWPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(APWPlayerCharacter, bIsDead, COND_None, REPNOTIFY_Always);
}

void APWPlayerCharacter::PlayMontage(TSoftObjectPtr<UAnimMontage> AnimMontage)
{
	if (AnimMontage.IsNull() == true)
	{
		ensure(false);
		return;
	}

	if (HasAuthority() == true)
	{
		SM_PlayMontage(AnimMontage.LoadSynchronous());
	}
	else
	{
		CS_PlayMontage(AnimMontage.LoadSynchronous());
	}
}

void APWPlayerCharacter::StopMontage(TSoftObjectPtr<UAnimMontage> AnimMontage)
{
	if (AnimMontage.IsNull() == true)
	{
		ensure(false);
		return;
	}

	if (HasAuthority() == true)
	{
		SM_StopMontage(AnimMontage.LoadSynchronous());
	}
	else
	{
		CS_StopMontage(AnimMontage.LoadSynchronous());
	}
}

void APWPlayerCharacter::CS_PlayMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayMontage(AnimMontage);
}

void APWPlayerCharacter::SM_PlayMontage_Implementation(UAnimMontage* AnimMontage)
{
	PlayAnimMontage(AnimMontage);
}

void APWPlayerCharacter::CS_StopMontage_Implementation(UAnimMontage* AnimMontage)
{
	StopMontage(AnimMontage);
}

void APWPlayerCharacter::SM_StopMontage_Implementation(UAnimMontage* AnimMontage)
{
	StopAnimMontage(AnimMontage);
}

void APWPlayerCharacter::SM_EnableCharacterAnimation_Implementation(bool bEnabled)
{
	if (bEnabled == true)
	{
		CustomTimeDilation = 1.f;
	}
	else
	{
		//움직임 정지
		if (IsValid(GetCharacterMovement()) == true)
		{
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			GetCharacterMovement()->UpdateComponentVelocity();
		}

		// 캐릭터 포즈 그대로 즉시 행동 멈추게
		CustomTimeDilation = 0.f;
	}
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

	SM_EnableCharacterAnimation(true);

	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterDeadDelegate.Broadcast(this);
	}

	//LookAt
	const FRotator& LookRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cast<AActor>(Killer)->GetActorLocation());
	SetActorRotation(LookRotator);

	PlayMontage(UPWAnimDataAsset::GetAnimMontage(this, EAnimMontageType::Death));

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == true)
	{
		 SetLifeSpan(PWGameSetting->DeathLifeSpan);
	}
}

UPWAttributeSet_Healable* APWPlayerCharacter::GetPWAttributeSet_Healable() const
{
	return PWAttributeSet_Healable;
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

void APWPlayerCharacter::CS_GiveDamage_Implementation(const TScriptInterface<IPWDamageableInterface>& Victim, float Damage)
{
	APWPlayerCharacter* VictimCharacter = Cast<APWPlayerCharacter>(Victim.GetObject());
	if (IsValid(VictimCharacter) == false)
	{
		ensure(false);
		return;
	}

	//Apply Damage
	VictimCharacter->ApplyDamage(this, Damage);
}

void APWPlayerCharacter::SM_OnLifeSpanExpired_Implementation()
{
	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->OnDeath();
	}
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