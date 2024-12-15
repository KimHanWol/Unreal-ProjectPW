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
#include "Component/PWCharacterSightComponent.h"
#include "Component/PWEquipmentComponent.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Engine/SpotLight.h"
#include "Helper/PWGameplayStatics.h"
#include "PWPlayerController.h"


APWPlayerCharacter::APWPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	

	PWEquipmentComponent = CreateDefaultSubobject<UPWEquipmentComponent>(TEXT("EquipmentComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemCompnent"));
	CharacterHUDComponent = CreateDefaultSubobject<UPWCharacterHUDComponent>(TEXT("CharacterHUDComponent"));
	CharacterHUDComponent->SetupAttachment(GetRootComponent());

	CharacterSightComponent = CreateDefaultSubobject<UPWCharacterSightComponent>(TEXT("CharacterSightComponent"));
	CharacterSightComponent->SetupAttachment(GetRootComponent());

	PWAttributeSet_Attackable = CreateDefaultSubobject<UPWAttributeSet_Attackable>(TEXT("AttributeSet_Attackable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Attackable);

	PWAttributeSet_Damageable = CreateDefaultSubobject<UPWAttributeSet_Damageable>(TEXT("AttributeSet_Damageable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Damageable);

	PWAttributeSet_Healable = CreateDefaultSubobject<UPWAttributeSet_Healable>(TEXT("AttributeSet_Healable"));
	AbilitySystemComponent->AddAttributeSetSubobject(PWAttributeSet_Healable);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void APWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &APWPlayerCharacter::OnLocalCharacterPossessed);
	}
}

void APWPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
	}

	//Set attribute
	WithdrawAttributeData();
}

void APWPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PrevLocation == FVector::ZeroVector)
	{
		PrevLocation = GetActorLocation();
		return;
	}

	APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
	if (IsValid(PWPlayerState) == false)
	{
		return;
	}

	const FVector& CurrentLocation = GetActorLocation();
    float DistanceMoved = FVector::Dist(CurrentLocation, PrevLocation);
    PrevLocation = CurrentLocation;

	if (DistanceMoved > 0.f)
	{
		//스냅샷 적용중이면 무시
		if (bIgnoreMoveRecordForNextTick == true)
		{
			bIgnoreMoveRecordForNextTick = false;
			return;
		}
		PWPlayerState->OnCharacterMoved(DistanceMoved / 100.f);
	}
}

void APWPlayerCharacter::PossessedBy(class AController* NewController)
{
	Super::PossessedBy(NewController);

	SM_EnableCharacterAnimation(true);
}

void APWPlayerCharacter::UnPossessed()
{
	PrevLocation = FVector::ZeroVector;
	SM_EnableCharacterAnimation(false);

	Super::UnPossessed();
}

void APWPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(APWPlayerCharacter, bIsDead, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(APWPlayerCharacter, TeamSide, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(APWPlayerCharacter, CharacterType, COND_None, REPNOTIFY_Always);
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

void APWPlayerCharacter::SM_EnableCharacterAnimation_Implementation(bool bPossessed)
{
	EnableCharacterAnimation(bPossessed);
}

void APWPlayerCharacter::SM_ApplySnapshotTransform_Implementation(const FTransform& NewTransform)
{
	//클라에서 움직임 딜레이가 생겨서 스냅샷 적용 이동이 행동력을 써버림
	//딜레이 생기지 않도록 직접 이동시켜줌
	bIgnoreMoveRecordForNextTick = true;
	SetActorTransform(NewTransform);
}

void APWPlayerCharacter::SM_InitializeCharacter_Implementation(APWPlayerController* OwnerController, ECharacterType InCharacterType)
{
	CharacterType = InCharacterType;
	bIsLocalCharacter = IsValid(OwnerController) == true && OwnerController->IsLocalController() == true;
	
	if (const FPWCharacterDataTableRow* CharacterData = UPWGameplayStatics::FindCharacterData(this, CharacterType))
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
		if (IsValid(SkeletalMeshComponent) == true)
		{
			SkeletalMeshComponent->SetSkeletalMesh(CharacterData->Mesh.LoadSynchronous());
		}

		if (IsValid(PWEquipmentComponent) == true)
		{
			PWEquipmentComponent->SpawnEquipmentActor(CharacterType);
		}
	}
}

void APWPlayerCharacter::SM_EnableCharacterSpotLight_Implementation(bool bEnabled)
{
	if(bIsLocalCharacter == false) 
	{
		return;
	}

	CharacterSightComponent->EnableSpotLight(bEnabled);
}

void APWPlayerCharacter::SM_HideActorByAliveState_Implementation(bool bHide)
{
	if (HasAuthority() == true)
	{
		SetActorHiddenInGame(bHide);
	}

	if (IsValid(PWEquipmentComponent) == true)
	{
		PWEquipmentComponent->OnAliveStateChanged(bHide);
	}
}

void APWPlayerCharacter::EnableCharacterAnimation(bool bEnabled)
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
	//On character death
	if (bIsDead == true)
	{
		return;
	}
	bIsDead = true;

	SM_EnableCharacterAnimation(true);
	SM_EnableCharacterSpotLight(false);

	//LookAt
	LastRotationBeforeDeath = GetActorRotation();
	const FRotator& LookRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Cast<AActor>(Killer)->GetActorLocation());
	SetActorRotation(LookRotator);

	SetActorEnableCollision(false);

	PlayMontage(UPWAnimDataAsset::GetAnimMontage(this, CharacterType, EAnimMontageType::Death));

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == true)
	{
		GetWorldTimerManager().SetTimer(DeathLifeSpanWaitTimerHandle, FTimerDelegate::CreateLambda([WeakThis = TWeakObjectPtr<APWPlayerCharacter>(this)](){
			if (WeakThis.IsValid())
			{
				WeakThis->SM_HideActorByAliveState(true);
			}
		}), PWGameSetting->DeathLifeSpan, false);
	}

	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterAliveStateChangedDelegate.Broadcast(this, false);
	}
}

void APWPlayerCharacter::OnPlayerRevived()
{
	//On character revive
	bIsDead = false;
	GetWorldTimerManager().ClearTimer(DeathLifeSpanWaitTimerHandle);

	SetActorRotation(LastRotationBeforeDeath);
	SetActorEnableCollision(true);

	SM_EnableCharacterAnimation(false);
	SM_HideActorByAliveState(false);
	SM_EnableCharacterSpotLight(true);

	const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterAliveStateChangedDelegate.Broadcast(this, true);
	}
}

UPWAttributeSet_Healable* APWPlayerCharacter::GetPWAttributeSet_Healable() const
{
	return PWAttributeSet_Healable;
}

void APWPlayerCharacter::InitializeCharacter(class APWPlayerController* OwnerPlayerController, ETeamSide NewTeamSide, ECharacterType NewCharacterType)
{
	TeamSide = NewTeamSide;
	CharacterType = NewCharacterType;

	//Set attribute
	ApplyAttributeData();

	SM_InitializeCharacter(OwnerPlayerController, NewCharacterType);
	SM_EnableCharacterSpotLight(true);
}

const FPWCharacterDataTableRow* APWPlayerCharacter::GetCharacterData() const
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return nullptr;
	}

	TArray<FPWCharacterDataTableRow*> PWCharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	for (FPWCharacterDataTableRow* PWCharacterData : PWCharacterDataList)
	{
		if (PWCharacterData->CharacterType == CharacterType)
		{
			return PWCharacterData;
		}
	}

	return nullptr;
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

void APWPlayerCharacter::LoadCharacterDefaultStats()
{
	if (const FPWCharacterDataTableRow* PWCharacterDataTableRow = GetCharacterData())
	{
		FString LogString;

		LogString.Append(FString::Printf(TEXT("%s Attribute Initialized with %s data: \n"), *GetName(), *UPWGameplayStatics::ConvertEnumToString(this, CharacterType)));
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

void APWPlayerCharacter::ApplyAttributeData()
{
	if (IsValid(PWAttributeSet_Damageable) == true)
	{
		PWAttributeSet_Damageable->BindAttributeChanged();
	}

	LoadCharacterDefaultStats();
}

void APWPlayerCharacter::WithdrawAttributeData()
{
	if (IsValid(PWAttributeSet_Damageable) == true)
	{
		PWAttributeSet_Damageable->UnbindAttributeChanged();
	}
}

void APWPlayerCharacter::OnLocalCharacterPossessed(APawn* PossessedPawn, bool bIsCommander)
{
	if (bIsCommander == true)
	{
		return;
	}

	if (PossessedPawn == this)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}
