// LINK

//Default
#include "MainWidget_CharacterButton.h"

//Engine
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Core/PWAssetLoadManager.h"
#include "Core/PWEventManager.h"
#include "Core/PWPlayerState.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Helper/PWGameplayStatics.h"

void UMainWidget_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(OwnedPlayerCharacter) == true)
	{
		const FPWCharacterDataTableRow* CharacterDataTableRow = OwnedPlayerCharacter->GetCharacterData();
		if (CharacterDataTableRow != nullptr)
		{
			UPWAssetLoadManager::AsyncLoad(this, CharacterDataTableRow->Portrait);
		}
	}
}

void UMainWidget_CharacterButton::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->HealthChangedDelegate.AddUObject(this, &UMainWidget_CharacterButton::OnHealthChanged);
	}
}

void UMainWidget_CharacterButton::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->HealthChangedDelegate.RemoveAll(this);
	}
}

void UMainWidget_CharacterButton::InvalidateWidget()
{
	Super::InvalidateWidget();

	if (IsValid(OwnedPlayerCharacter) == false)
	{
		return;
	}

	const FPWCharacterDataTableRow* CharacterDataTableRow = OwnedPlayerCharacter->GetCharacterData();
	if (CharacterDataTableRow == nullptr)
	{
		ensure(false);
		return;
	}

	if (IsValid(Text_Num) == true)
	{
		Text_Num->SetText(FText::FromString(FString::FromInt(ButtonIndex + 1)));
	}

	if (IsValid(Text_Name) == true)
	{
		Text_Name->SetText(FText::FromName(CharacterDataTableRow->DisplayName));
	}

	if (IsValid(Image_Portrait) == true)
	{
		Image_Portrait->SetBrushFromSoftTexture(CharacterDataTableRow->Portrait);
	}
}

void UMainWidget_CharacterButton::InitializeCharacterButton(APWPlayerCharacter* InOwnedPlayerCharacter)
{
	OwnedPlayerCharacter = InOwnedPlayerCharacter;
	InvalidateWidget();
}

FReply UMainWidget_CharacterButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const APWPlayerState* PWPlayerState = UPWGameplayStatics::GetLocalPlayerState(this);
	if (PWPlayerState->IsMyTurn() == false)
	{
		return FReply::Unhandled();
	}

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->CharacterSelectedDelegate.Broadcast(ButtonIndex + 1);
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UMainWidget_CharacterButton::OnHealthChanged(AActor* TargetActor, float InMaxHealth, float InCurrentHealth)
{
	if (IsValid(OwnedPlayerCharacter) == false || OwnedPlayerCharacter != TargetActor)
	{
		return;
	}

	if (IsValid(PBar_Health) == true)
	{
		PBar_Health->SetPercent(InCurrentHealth / InMaxHealth);
	}

	if (InCurrentHealth <= 0.f)
	{
		if (ensure(IsValid(Overlay_Dead) == true))
		{
			Overlay_Dead->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (ensure(IsValid(Overlay_Dead) == true))
		{
			Overlay_Dead->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}