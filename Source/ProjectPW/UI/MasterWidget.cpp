// LINK

//Default
#include "MasterWidget.h"

//Engine

//Game
#include "Core/PWEventManager.h"
#include "Helper/PWGameplayStatics.h"
#include "UI/CharacterSelection/CharacterSelectionWidget.h"
#include "UI/Loading/LoadingWidget.h"
#include "UI/Main/MainWidget.h"

void UMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WidgetList.Add(MainWidget);
	WidgetList.Add(CharacterSelectionWidget);

	ShowWidget(EWidgetType::CharacterSelection);
}

void UMasterWidget::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->ShowWidgetDelegate.AddUObject(this, &UMasterWidget::ShowWidget);
	}
}

void UMasterWidget::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->ShowWidgetDelegate.RemoveAll(this);
	}
}

void UMasterWidget::HideAllWidget()
{
	for (UPWUserWidget* UserWidget : WidgetList)
	{
		if (IsValid(UserWidget) == false)
		{
			ensure(false);
			continue;
		}

		UserWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMasterWidget::ShowWidget(EWidgetType TargetWidgetType)
{
	HideAllWidget();
	switch (TargetWidgetType)
	{
		case EWidgetType::CharacterSelection:
		{
			if (ensure(IsValid(CharacterSelectionWidget) == true))
			{
				CharacterSelectionWidget->SetVisibility(ESlateVisibility::Visible);
				CharacterSelectionWidget->InitializeCharacterSelectionWidget();
			}
			break;
		}
		case EWidgetType::MainWidget:
		{
			if (ensure(IsValid(MainWidget) == true))
			{
				MainWidget->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		}
		case EWidgetType::LoadingWidget:
		{
			if (ensure(IsValid(LoadingWidget) == true))
			{
				LoadingWidget->SetVisibility(ESlateVisibility::Visible);
				FName SelectedLevelKey = UPWGameplayStatics::GetSelectedLevelKey(this);
				LoadingWidget->InitializeLoadingWidget(SelectedLevelKey);
			}
			break;
		}
	}
}