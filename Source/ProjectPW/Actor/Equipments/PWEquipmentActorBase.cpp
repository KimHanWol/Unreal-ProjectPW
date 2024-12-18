// LINK

//Default
#include "PWEquipmentActorBase.h"

//Engine
#include "AbilitySystemComponent.h"

//Game
#include "AbilitySystem/Ability/PWGameplayAbilityBase.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWAssetLoadManager.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"

APWEquipmentActorBase::APWEquipmentActorBase(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APWEquipmentActorBase::BeginPlay()
{
	Super::BeginPlay();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &APWEquipmentActorBase::OnPlayerPossesssed);
	}
}

void APWEquipmentActorBase::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
	}
}

void APWEquipmentActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckTargetOnCrosshair();
}

void APWEquipmentActorBase::InitializeEquipmentActor(AActor* OwnerActor)
{
	SetOwner(OwnerActor);

	const APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(OwnerActor);
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	if (OwnerCharacter->HasAuthority() == false)
	{
		return;
	}
	
	UAbilitySystemComponent* OwnerASC = OwnerCharacter->GetAbilitySystemComponent();
	if (IsValid(OwnerASC) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(MainTriggerAbilityClass) == true)
	{
		FGameplayAbilitySpec MainAbilitySpec(MainTriggerAbilityClass);
		OwnerASC->GiveAbility(MainAbilitySpec);
	}

	if (IsValid(MainCompleteAbilityClass) == true)
	{
		FGameplayAbilitySpec MainAbilitySpec(MainCompleteAbilityClass);
		OwnerASC->GiveAbility(MainAbilitySpec);
	}

	if (IsValid(SubTriggerAbilityClass) == true)
	{
		FGameplayAbilitySpec SubAbilitySpec(SubTriggerAbilityClass);
		OwnerASC->GiveAbility(SubAbilitySpec);
	}

	if (IsValid(SubCompleteAbilityClass) == true)
	{
		FGameplayAbilitySpec MainAbilitySpec(SubCompleteAbilityClass);
		OwnerASC->GiveAbility(MainAbilitySpec);
	}
}

void APWEquipmentActorBase::Execute_Main_Triggered()
{
	if (bIsMainActionActivated == true)
	{
		return;
	}

	bIsMainActionActivated = true;

	if (IsValid(MainTriggerAbilityClass) == true)
	{
		TryExecute_Action_Internal(true, MainTriggerAbilityClass);
	}
}

void APWEquipmentActorBase::Execute_Main_Completed()
{
	if (IsValid(MainTriggerAbilityClass) == true)
	{
		TryExecute_Action_Internal(false, MainTriggerAbilityClass);
	}

	if (IsValid(MainCompleteAbilityClass) == true)
	{
		TryExecute_Action_Internal(true, MainCompleteAbilityClass);
	}

	bIsMainActionActivated = false;
}

void APWEquipmentActorBase::Execute_Sub_Triggered()
{
	if (bIsSubActionActivated == true)
	{
		return;
	}
	bIsSubActionActivated = true;

	if (IsValid(SubTriggerAbilityClass) == true)
	{
		TryExecute_Action_Internal(true, SubTriggerAbilityClass);
	}
}

void APWEquipmentActorBase::Execute_Sub_Completed()
{
	if (IsValid(SubTriggerAbilityClass) == true)
	{
		TryExecute_Action_Internal(false, SubTriggerAbilityClass);
	}

	if (IsValid(SubCompleteAbilityClass) == true)
	{
		TryExecute_Action_Internal(true, SubCompleteAbilityClass);
	}

	bIsSubActionActivated = false;
}

bool APWEquipmentActorBase::EquipmentActorHitTest(float InMaxRange, ECollisionChannel TargetChannel, FHitResult& OutHitResult, FRotator& OutViewPointRotation)
{
	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return false;
	}
		
	AController* OwnerController = OwnerCharacter->GetController();
	if (IsValid(OwnerController) == false)
	{
		return false;
	}

	bool bHitSuccess = false;
	FVector ViewPointLocation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, OutViewPointRotation);

	const FVector& EndLocation = ViewPointLocation + OutViewPointRotation.Vector() * InMaxRange;

	if (IsValid(GetWorld()) == true)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		bHitSuccess = GetWorld()->LineTraceSingleByChannel(OutHitResult, ViewPointLocation, EndLocation, TargetChannel, Params);
	}

	return bHitSuccess;
}

void APWEquipmentActorBase::CheckTargetOnCrosshair()
{
	FHitResult HitResult;
	FRotator ViewPointRotation;

	float MaxRange = 0.f;
	if (ensure(IsValid(MainTriggerAbilityClass) == true))
	{
		const UPWGameplayAbilityBase* MainAbilityCDO = Cast<UPWGameplayAbilityBase>(MainTriggerAbilityClass->GetDefaultObject());
		if (ensure(IsValid(MainAbilityCDO) == true)) 
		{
			MaxRange = MainAbilityCDO->GetMaxRange();
		}
	}

	bool bHitSuccess = EquipmentActorHitTest(MaxRange, ECollisionChannel::ECC_GameTraceChannel1, HitResult, ViewPointRotation);
	bool bHitOnDemageableActor = bHitSuccess && IsInteractableActor(HitResult.GetActor());

	if (bHitOnDemageableActor != bIsTargetOn)
	{
		bIsTargetOn = bHitOnDemageableActor;

		const UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TargetIsOnCrosshairDelegate.Broadcast(Cast<APWPlayerCharacter>(GetOwner()), bIsTargetOn);
		}
	}
}

void APWEquipmentActorBase::OnPlayerPossesssed(APawn* PossessedPawn, bool bIsCommander)
{
	if (IsValid(PossessedPawn) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return;
	}

	if (PossessedPawn == OwnerCharacter)
	{
		SetActorTickEnabled(true);
	}
	else
	{
		SetActorTickEnabled(false);
	}
}

void APWEquipmentActorBase::TryExecute_Action_Internal(bool bExecute, TSubclassOf<class UPWGameplayAbilityBase> TargetAbility)
{
	if (IsValid(TargetAbility) == false)
	{
		ensure(false);
		return;
	}

 	const UPWGameplayAbilityBase* TargetAbilityCDO = Cast<UPWGameplayAbilityBase>(TargetAbility->GetDefaultObject());
	if (IsValid(TargetAbilityCDO) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		return;
	}

	APWPlayerState* OwnerPlayerState = OwnerCharacter->GetPlayerState<APWPlayerState>();
	if (IsValid(OwnerPlayerState) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerController* OwnerController = Cast<APWPlayerController>(OwnerCharacter->GetController());
	if (IsValid(OwnerController) == false)
	{
		ensure(false);
		return;
	}

	UAbilitySystemComponent* OwnerCharacterASC = OwnerCharacter->GetAbilitySystemComponent();
	if (IsValid(OwnerCharacterASC) == false)
	{
		ensure(false);
		return;
	}

	if (bExecute == true)
	{
		bool bHasEnoughTurnActivePoint = TargetAbilityCDO->GetTurnActivePointCost() <= OwnerPlayerState->GetCurrentTurnActivePoint();
		if (bHasEnoughTurnActivePoint == true)
		{
			bool bSuccess = OwnerCharacterASC->TryActivateAbilityByClass(TargetAbility);
			if (bSuccess == true)
			{
				OwnerPlayerState->SetCurrentTurnActivePoint(OwnerPlayerState->GetCurrentTurnActivePoint() - TargetAbilityCDO->GetTurnActivePointCost());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't activate %s ability : not enough current turn point."), *TargetAbility->GetName());
		}
	}
	else
	{
		for (const FGameplayAbilitySpec& AbilitySpec : OwnerCharacterASC->GetActivatableAbilities())
		{
		    if (AbilitySpec.IsActive() && IsValid(AbilitySpec.Ability) == true && AbilitySpec.Ability->GetClass() == TargetAbility)
		    {
		        OwnerCharacterASC->CancelAbility(AbilitySpec.Ability);
		    }
		}
	}
}
