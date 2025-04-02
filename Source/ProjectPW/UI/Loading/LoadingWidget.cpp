// LINK

//Default
#include "LoadingWidget.h"

//Engine
#include "Components/Image.h"
#include "Components/ProgressBar.h"

//Game
#include "Core/PWEventManager.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/DataTable/PWLevelDataTableRow.h"
#include "UI/MasterWidget.h"

void ULoadingWidget::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->BattleLevelSettingFinished.AddUObject(this, &ULoadingWidget::OnBattleLevelSettingFinished);
	}
}

void ULoadingWidget::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->BattleLevelSettingFinished.RemoveAll(this);
	}
}

void ULoadingWidget::InvalidateWidget()
{
	Super::InvalidateWidget();
}

void ULoadingWidget::InitializeLoadingWidget(FName LevelKey)
{
	const FPWLevelDataTableRow* LoadingLevelData = UPWGameData::FindLevelTableRow(this, LevelKey);
	if (LoadingLevelData == nullptr)
	{
		ensureMsgf(false, TEXT("Cannot find level data, Please check level key or level data table."));
		return;
	}

#if WITH_EDITOR
	bIsBattleLevelSettingFinished = true;
#else 
	bIsBattleLevelSettingFinished = false;
#endif //WITH_EDITOR

	if (ensure(IsValid(Text_MapName) == true))
	{
		Text_MapName->SetText(FText::FromName(LoadingLevelData->DisplayName));
	}

	if (ensure(IsValid(Image_Background) == true))
	{
		Image_Background->SetBrushFromSoftTexture(LoadingLevelData->LevelImage);
	}

	GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]() {

		float Alpha = CurrentLoadingDuration / MinLoadingDuration;
		float Progress = FMath::Lerp(0.f, 1.f, Alpha);

		if (ensure(IsValid(ProgressBar_Loading) == true))
		{
			ProgressBar_Loading->SetPercent(Progress);
		}

		if (CurrentLoadingDuration >= MinLoadingDuration)
		{
			GetWorld()->GetTimerManager().ClearTimer(LoadingTimerHandle);
			OnLoadingFinished();
		}

		// 모든 플레이어가 준비 되기 전에는 중간에 멈추게 함
		if (Progress < WaitingOtherPlayerReadyPoint || bIsBattleLevelSettingFinished == true)
		{
			CurrentLoadingDuration += 0.01f;
		}

		}), 0.01f, true);
}

void ULoadingWidget::OnBattleLevelSettingFinished()
{
	bIsBattleLevelSettingFinished = true;
}

void ULoadingWidget::OnLoadingFinished()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->ShowWidgetDelegate.Broadcast(EWidgetType::CharacterSelection);
	}
}
