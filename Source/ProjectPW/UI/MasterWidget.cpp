// LINK

//Default
#include "MasterWidget.h"

//Engine

//Game
#include "UI/Main/MainWidget.h"

void UMasterWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(MainWidget) == true)
	{
		MainWidget->SetVisibility(ESlateVisibility::Visible);
	}
}