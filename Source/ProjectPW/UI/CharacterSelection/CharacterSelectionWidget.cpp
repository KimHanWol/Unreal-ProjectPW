// LINK

//Default
#include "CharacterSelectionWidget.h"

//Engine
#include "Components/Overlay.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetMathLibrary.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "CharacterSelectionButton.h"
#include "Core/PWEventManager.h"
#include "Core/PWGameInstance.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataAsset/PWGameData.h"
#include "Data/PWGameEnum.h"
#include "Helper/PWGameplayStatics.h"


void UCharacterSelectionWidget::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (ensure(IsValid(PWEventManager) == true))
	{
		PWEventManager->CharacterSelectedForSpawnDelegate.AddUObject(this, &UCharacterSelectionWidget::OnSelectedCharacterChanged);
		PWEventManager->TeamCharacterAllSpawnedDelegate.AddUObject(this, &UCharacterSelectionWidget::OnPlayerCharacterAllSpawned);
	}
}

void UCharacterSelectionWidget::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (ensure(IsValid(PWEventManager) == true))
	{
		PWEventManager->CharacterSelectedForSpawnDelegate.RemoveAll(this);
		PWEventManager->TeamCharacterAllSpawnedDelegate.RemoveAll(this);
	}
}

void UCharacterSelectionWidget::InitializeCharacterSelectionWidget()
{
	const UPWGameData* PWGameData = UPWGameData::Get(this);
	if (IsValid(PWGameData) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(VBox_CharacterClass) == false)
	{
		ensure(false);
		return;
	}

	const TArray<FPWCharacterDataTableRow*>& CharacterDataList = PWGameData->GetAllTableRow<FPWCharacterDataTableRow>(EDataTableType::Character);
	for (int32 i = 0; i < CharacterDataList.Num(); i++)
	{
		if (const FPWCharacterDataTableRow* CharacterData = CharacterDataList[i])
		{
			UCharacterSelectionButton* NewSelectionWidget = CreateWidget<UCharacterSelectionButton>(this, CharacterSelectionButtonClass);
			if (IsValid(NewSelectionWidget) == true)
			{
				NewSelectionWidget->InitializeSelectionWidget(i + 1, CharacterData);
				NewSelectionWidget->WidgetClickedDelegate.AddUObject(this, &UCharacterSelectionWidget::OnSelectionWidgetClicked);
				VBox_CharacterClass->AddChildToVerticalBox(NewSelectionWidget);
				SelectionWidgetList.Add(NewSelectionWidget);
			}

			USpacer* NewSpacer = NewObject<USpacer>(this);
			if (IsValid(NewSpacer) == true)
			{
				NewSpacer->SetSize(FVector2D(0.f, CharacterSelectionButtonSpaceOffset));
				VBox_CharacterClass->AddChildToVerticalBox(NewSpacer);
			}
			ChildrenCharacterDataList.Add(*CharacterData);

			// 처음은 0 번으로 초기화
			if (i == 0)
			{
				UPWEventManager* PWEventManager = UPWEventManager::Get(this);
				if (ensure(IsValid(PWEventManager) == true))
				{
					PWEventManager->CharacterSelectedForSpawnDelegate.Broadcast(CharacterData->CharacterType);
				}
			}
		}
	}

	
	APWPlayerController* LocalPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
	if (IsValid(LocalPlayerController) == true)
	{
		ETeamSide TeamSide = LocalPlayerController->GetTeamSide();
		if (ensure(IsValid(Text_TeamSide) == true))
		{
			if (TeamSide == ETeamSide::Red)
			{
				Text_TeamSide->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f));
			}
			else
			{
				Text_TeamSide->SetColorAndOpacity(FLinearColor(0.f, 0.f, 1.f));
			}
			Text_TeamSide->SetText(FText::FromString(UPWGameplayStatics::ConvertEnumToString<ETeamSide>(this, LocalPlayerController->GetTeamSide()) + FString(" Team")));
		}
	}
}

void UCharacterSelectionWidget::OnSelectedCharacterChanged(ECharacterType SelectedChracterType)
{
	for (int32 i = 0; i < ChildrenCharacterDataList.Num(); i++)
	{
		const FPWCharacterDataTableRow* CharacterData = &ChildrenCharacterDataList[i];
		if (CharacterData->CharacterType == SelectedChracterType)
		{
			if (CurrentSelectedIndex != i)
			{
				PlaySelectAnimation(i);
			}
			break;
		}
	}
}

void UCharacterSelectionWidget::OnPlayerCharacterAllSpawned(const APWPlayerController* TargetPlayerController, const TArray<TWeakObjectPtr<class APWPlayerCharacter>>& TargetCharacterList)
{
	if (IsValid(TargetPlayerController) == false || TargetPlayerController->IsMyController() == false)
	{
		return;
	}

	if (ensure(IsValid(Overlay_Selection) == true))
	{
		Overlay_Selection->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ensure(IsValid(Overlay_Complete) == true))
	{
		Overlay_Complete->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UCharacterSelectionWidget::OnSelectionWidgetClicked(int32 SelectedWidgetNum)
{
	if (CurrentSelectedIndex == SelectedWidgetNum - 1)
	{
		return;
	}

	PlaySelectAnimation(SelectedWidgetNum - 1);

	if (const FPWCharacterDataTableRow* CharacterData = &ChildrenCharacterDataList[SelectedWidgetNum - 1])
	{
		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->CharacterSelectedForSpawnDelegate.Broadcast(CharacterData->CharacterType);
		}
	}
}

void UCharacterSelectionWidget::PlaySelectAnimation(int32 NewSelectIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(SelectAnimationTimerHandle);
	for (int32 i = 0; i < SelectionWidgetList.Num(); i++)
	{
		GetWorld()->GetTimerManager().ClearTimer(DeselectAnimationTimerHandleMap.FindOrAdd(i));
	}

	if (NewSelectIndex < 0 || SelectionWidgetList.Num() <= NewSelectIndex)
	{
		ensure(false);
		return;
	}

	// 다른 건 전부 제자리로
	for (int32 i = 0; i < SelectionWidgetList.Num(); i++)
	{
		if (i != NewSelectIndex)
		{
			PlayDeselectAnimation(i);
		}
	}
	CurrentSelectedIndex = NewSelectIndex;

	UWidget* ChildWidget = SelectionWidgetList[NewSelectIndex];
	if (IsValid(ChildWidget) == true)
	{
		float TimeElapsed = 0.0f;
		FVector2D StartPosition = ChildWidget->GetRenderTransform().Translation;
		FVector2D TargetPosition = FVector2D(70.f, 0.f);

		GetWorld()->GetTimerManager().SetTimer(SelectAnimationTimerHandle, FTimerDelegate::CreateLambda([this, NewSelectIndex, ChildWidget, StartPosition, TargetPosition, TimeElapsed]() mutable
			{
				TimeElapsed += 0.01f;
				float Alpha = FMath::Clamp(TimeElapsed / SlideAnimaionDuration, 0.0f, 1.0f);
				FVector NewPos = UKismetMathLibrary::VLerp(FVector(StartPosition, 0.f), FVector(TargetPosition, 0.f), Alpha);
				FWidgetTransform NewTransform = ChildWidget->GetRenderTransform();
				NewTransform.Translation = FVector2D(NewPos);
				ChildWidget->SetRenderTransform(NewTransform);

				if (Alpha >= 1.0f || CurrentSelectedIndex != NewSelectIndex)
				{
					GetWorld()->GetTimerManager().ClearTimer(SelectAnimationTimerHandle);
				}

			}), 0.01f, true);
	}
}

void UCharacterSelectionWidget::PlayDeselectAnimation(int32 TargetIndex)
{
	if (TargetIndex >= 0 && ensure(SelectionWidgetList.Num() > TargetIndex))
	{
		UWidget* ChildWidget = SelectionWidgetList[TargetIndex];
		if (IsValid(ChildWidget) == true)
		{
			float TimeElapsed = 0.0f;
			FVector2D StartPosition = ChildWidget->GetRenderTransform().Translation;

			if (StartPosition.IsNearlyZero() == true)
			{
				return;
			}

			FVector2D TargetPosition = FVector2D(0.f, 0.f);

			GetWorld()->GetTimerManager().SetTimer(DeselectAnimationTimerHandleMap.FindOrAdd(TargetIndex), FTimerDelegate::CreateLambda([this, TargetIndex, ChildWidget, StartPosition, TargetPosition, TimeElapsed]() mutable
				{
					TimeElapsed += 0.01f;
					float Alpha = FMath::Clamp(TimeElapsed / SlideAnimaionDuration, 0.0f, 1.0f);
					FVector NewPos = UKismetMathLibrary::VLerp(FVector(StartPosition, 0.f), FVector(TargetPosition, 0.f), Alpha);
					FWidgetTransform NewTransform = ChildWidget->GetRenderTransform();
					NewTransform.Translation = FVector2D(NewPos);
					ChildWidget->SetRenderTransform(NewTransform);

					if (Alpha >= 1.0f)
					{
						GetWorld()->GetTimerManager().ClearTimer(DeselectAnimationTimerHandleMap.FindOrAdd(TargetIndex));
					}
				}), 0.01f, true);
		}
	}
}