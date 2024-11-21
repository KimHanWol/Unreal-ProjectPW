// LINK

//Default
#include "MainWidget_CharacterButton.h"

//Engine
#include "Components/Image.h"
#include "Components/ProgressBar.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Core/PWEventManager.h"
#include "Data/PWGameEnum.h"
#include "Core/PWPlayerState.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Helper/PWGameplayStatics.h"

void UMainWidget_CharacterButton::InitializeCharacterButton(APWPlayerCharacter* InOwnedPlayerCharacter)
{
	OwnedPlayerCharacter = InOwnedPlayerCharacter;
	InvalidateWidget();

	//체력은 따로 바인딩해줘서 여기서 초기화
	if (IsValid(PBar_Health) == true)
	{
		PBar_Health->SetPercent(1.f);
	}
}

FReply UMainWidget_CharacterButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//TODO: 키보드 입력과 합칠 수 있으면 좋을 듯 (예: 이벤트시스템)
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == false)
	{
		ensure(false);
		return FReply::Unhandled();
	}

	const APWPlayerState* PWPlayerState = LocalPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == false)
	{
		ensure(false);
		return FReply::Unhandled();
	}

	if (PWPlayerState->IsMyTurn() == false)
	{
		return FReply::Unhandled();
	}

	LocalPlayerController->SelectCharacter(ButtonIndex + 1);

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
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

	if (IsValid(Text_Damage) == true)
	{
		Text_Damage->SetText(FText::FromString(TEXT("공격력 : ") + FString::FromInt(CharacterDataTableRow->Damage)));
	}

	if (IsValid(Text_Health) == true)
	{
		Text_Health->SetText(FText::FromString(TEXT("총 체력 : ") + FString::FromInt(CharacterDataTableRow->Health)));
	}

	if (IsValid(Text_HealAmount) == true)
	{
		Text_HealAmount->SetText(FText::FromString(TEXT("아군 회복량 : ") + FString::FromInt(CharacterDataTableRow->HealAmount)));
	}
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
}