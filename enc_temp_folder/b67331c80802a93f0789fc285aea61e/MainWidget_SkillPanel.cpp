// LINK

//Default
#include "MainWidget_SkillPanel.h"

//Engine
#include "Components/Image.h"

//Game
#include "AbilitySystem/Ability/PWGameplayAbilityBase.h"
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/Equipments/PWEquipmentActorBase.h"
#include "Core/PWAssetLoadManager.h"
#include "Core/PWEventManager.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataTable/PWEquipmentDataTableRow.h"
#include "Helper/PWGameplayStatics.h"

void UMainWidget_SkillPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (ensure(DefaultIconTexture.IsNull() == false))
	{
		UPWAssetLoadManager::AsyncLoad(this, DefaultIconTexture);
	}
}

void UMainWidget_SkillPanel::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (ensure(IsValid(PWEventManager) == true))
	{
		PWEventManager->PlayerPossessedDelegate.AddUObject(this, &UMainWidget_SkillPanel::OnPlayerPossessed);
	}
}

void UMainWidget_SkillPanel::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (ensure(IsValid(PWEventManager) == true))
	{
		PWEventManager->PlayerPossessedDelegate.RemoveAll(this);
	}
}

void UMainWidget_SkillPanel::InvalidateWidget()
{
	Super::InvalidateWidget();

	const FPWEquipmentDataTableRow* EquipmentData = UPWGameplayStatics::FindEquipmentData(this, PossessedCharacterType);
	if (EquipmentData == nullptr)
	{
		ensure(false);
		return;
	}

	const TSubclassOf<APWEquipmentActorBase> EquipmentActorClass = EquipmentData->EquipmentActor;
	if (IsValid(EquipmentActorClass) == false)
	{
		ensure(false);
		return;
	}

	const APWEquipmentActorBase* EquipmentActorCDO = Cast<APWEquipmentActorBase>(EquipmentActorClass->GetDefaultObject());
	if (IsValid(EquipmentActorCDO) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Image_Icon) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Text_Name) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(Text_Cost) == false)
	{
		ensure(false);
		return;
	}

	if (bIsForLeftMouseButton == true)
	{
		float TurnActivePointCost = 0.f;
		const TSubclassOf<class UPWGameplayAbilityBase> MainTriggerAbilityClass = EquipmentActorCDO->MainTriggerAbilityClass;
		const TSubclassOf<class UPWGameplayAbilityBase> MainCompleteAbilityClass = EquipmentActorCDO->MainCompleteAbilityClass;
		if (IsValid(MainTriggerAbilityClass) == true || IsValid(MainCompleteAbilityClass) == true)
		{
			FName SkillDisplayName = NAME_None;
			TSoftObjectPtr<UTexture2D> SkillIcon;

			if (IsValid(MainTriggerAbilityClass) == true)
			{
				const UPWGameplayAbilityBase* MainTriggerAbilityCDO = Cast<UPWGameplayAbilityBase>(MainTriggerAbilityClass->GetDefaultObject());
				if (IsValid(MainTriggerAbilityCDO) == true)
				{
					SkillDisplayName = MainTriggerAbilityCDO->SkillDisplayName;
					SkillIcon = MainTriggerAbilityCDO->SkillIcon;

					TurnActivePointCost += MainTriggerAbilityCDO->TurnActivePointCost;
				}
			}

			if (IsValid(MainCompleteAbilityClass) == true)
			{
				const UPWGameplayAbilityBase* MainCompleteAbilityCDO = Cast<UPWGameplayAbilityBase>(MainCompleteAbilityClass->GetDefaultObject());
				if (IsValid(MainCompleteAbilityCDO) == true)
				{
					if (SkillDisplayName.IsNone() == true)
					{
						SkillDisplayName = MainCompleteAbilityCDO->SkillDisplayName;
					}

					if (SkillIcon.IsNull() == true)
					{
						SkillIcon = MainCompleteAbilityCDO->SkillIcon;
					}

					TurnActivePointCost += MainCompleteAbilityCDO->TurnActivePointCost;
				}
			}
			
			Text_Name->SetText(FText::FromName(SkillDisplayName));
			Image_Icon->SetBrushFromSoftTexture(SkillIcon);
			Text_Cost->SetText(FText::FromString(FString::FromInt(TurnActivePointCost)));
		}
		else
		{
			Text_Name->SetText(FText::GetEmpty());
			Image_Icon->SetBrushFromSoftTexture(DefaultIconTexture.LoadSynchronous());
			Text_Cost->SetText(FText::GetEmpty());
		}
	}
	else
	{
		const TSubclassOf<class UPWGameplayAbilityBase> SubAbilityClass = EquipmentActorCDO->SubTriggerAbilityClass;
		if (IsValid(SubAbilityClass) == true)
		{
			const UPWGameplayAbilityBase* SubAbilityCDO = Cast<UPWGameplayAbilityBase>(SubAbilityClass->GetDefaultObject());
			if (ensure(IsValid(SubAbilityCDO) == true))
			{
				Text_Name->SetText(FText::FromName(SubAbilityCDO->SkillDisplayName));
				Image_Icon->SetBrushFromSoftTexture(SubAbilityCDO->SkillIcon);
				Text_Cost->SetText(FText::FromString(FString::FromInt(SubAbilityCDO->TurnActivePointCost)));
			}
		}
		else
		{
			Text_Name->SetText(FText::GetEmpty());
			Image_Icon->SetBrushFromSoftTexture(DefaultIconTexture.LoadSynchronous());
			Text_Cost->SetText(FText::GetEmpty());
		}
	}
}

void UMainWidget_SkillPanel::OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander)
{
	const APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false)
	{
		ensure(false);
		return;
	}

	APWPlayerCharacter* LocalPossessedCharacter = Cast<APWPlayerCharacter>(PossessedPawn);
	if (IsValid(LocalPossessedCharacter) == false)
	{
		return;
	}

	const FPWCharacterDataTableRow* PossessedCharacterData = LocalPossessedCharacter->GetCharacterData();
	if (PossessedCharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	PossessedCharacterType = PossessedCharacterData->CharacterType;

	InvalidateWidget();
}
