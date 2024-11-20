// LINK

//Default
#include "MasterWidget.h"

//Engine

//Game
#include "UI/Main/MainWidget.h"

void UMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WidgetList.Add(MainWidget);

	CloseAllWidget();

	EnableMainWidget(true);
}

void UMasterWidget::CloseAllWidget()
{
	for (UUserWidget* InWidget : WidgetList)
	{
		if (ensure(IsValid(InWidget) == true))
		{
			InWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UMasterWidget::EnableMainWidget(bool bEnabled)
{
	if (IsValid(MainWidget) == false)
	{
		return;
	}

	if (bEnabled == true)
	{
		MainWidget->InvalidateWidget();
		MainWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//MainWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}