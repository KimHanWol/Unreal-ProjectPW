// LINK

//Default
#include "CharacterSelectionWidget.h"

//Engine
#include "Components/Spacer.h"
#include "Kismet/KismetMathLibrary.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "CharacterSelectionButton.h"
#include "Components/VerticalBox.h"
#include "Core/PWEventManager.h"
#include "Core/PWGameInstance.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "Data/DataAsset/PWGameData.h"

void UCharacterSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (ensure(IsValid(PWEventManager) == true))
	{
		PWEventManager->CharacterSelectedForSpawnDelegate.AddUObject(this, &UCharacterSelectionWidget::OnSelectedCharacterChanged);
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

void UCharacterSelectionWidget::PlaySelectAnimation(int32 NewSelectIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(SelectAnimationTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DeselectAnimationTimerHandle);

	if (NewSelectIndex >= 0 && ensure(SelectionWidgetList.Num() > NewSelectIndex))
	{
		PlayDeselectAnimation(CurrentSelectedIndex);
		CurrentSelectedIndex = NewSelectIndex;

		UWidget* ChildWidget = SelectionWidgetList[NewSelectIndex];
		if (IsValid(ChildWidget) == true)
		{
			float TimeElapsed = 0.0f;
			FVector2D TargetPosition = ChildWidget->GetRenderTransform().Translation + FVector2D(70.f, 0.f);

			GetWorld()->GetTimerManager().SetTimer(SelectAnimationTimerHandle, FTimerDelegate::CreateLambda([this, NewSelectIndex, ChildWidget, TargetPosition, &TimeElapsed]() mutable
				{
					TimeElapsed += 0.01f;
					float Alpha = FMath::Clamp(TimeElapsed / SlideAnimaionDuration, 0.0f, 1.0f);
					FVector NewPos = UKismetMathLibrary::VLerp(FVector(ChildWidget->GetRenderTransform().Translation, 0.f), FVector(TargetPosition, 0.f), Alpha);
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
}

void UCharacterSelectionWidget::PlayDeselectAnimation(int32 SelectIndex)
{
	if (SelectIndex >= 0 && ensure(SelectionWidgetList.Num() > SelectIndex))
	{
		UWidget* ChildWidget = SelectionWidgetList[SelectIndex];
		if (IsValid(ChildWidget) == true)
		{
			float TimeElapsed = 0.0f;
			FVector2D TargetPosition = FVector2D(0.f, 0.f);

			GetWorld()->GetTimerManager().SetTimer(DeselectAnimationTimerHandle, FTimerDelegate::CreateLambda([this, SelectIndex, ChildWidget, TargetPosition, &TimeElapsed]() mutable
				{
					TimeElapsed += 0.01f;
					float Alpha = FMath::Clamp(TimeElapsed / SlideAnimaionDuration, 0.0f, 1.0f);
					FVector NewPos = UKismetMathLibrary::VLerp(FVector(ChildWidget->GetRenderTransform().Translation, 0.f), FVector(TargetPosition, 0.f), Alpha);
					FWidgetTransform NewTransform = ChildWidget->GetRenderTransform();
					NewTransform.Translation = FVector2D(NewPos);
					ChildWidget->SetRenderTransform(NewTransform);

					if (Alpha >= 1.0f || CurrentSelectedIndex == SelectIndex)
					{
						GetWorld()->GetTimerManager().ClearTimer(DeselectAnimationTimerHandle);
					}
				}), 0.01f, true);
		}
	}
}
