// LINK

//Default
#include "MainMenuPopUp.h"

//Engine

//Game
#include "Core/PWEventManager.h"

void UMainMenuPopUp::BindEvents()
{
	Super::BindEvents();

	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->ShowNotiWidgetDelegate.AddUObject(this, &UMainMenuPopUp::OnShowNotiWidget);
	}
}

void UMainMenuPopUp::UnbindEvents()
{
	Super::UnbindEvents();

	UPWEventManager* EventManager = UPWEventManager::Get(this);
	if (IsValid(EventManager) == true)
	{
		EventManager->ShowNotiWidgetDelegate.RemoveAll(this);
	}
}

void UMainMenuPopUp::OnShowNotiWidget(const FText& NotificationText)
{
	if (ensure(IsValid(Text_Noti) == true))
	{
		Text_Noti->SetText(NotificationText);
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}