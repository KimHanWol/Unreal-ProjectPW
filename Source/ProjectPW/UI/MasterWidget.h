// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "UI/PWUserWidget.h"

//Game

//Default
#include "MasterWidget.generated.h"

UENUM()
enum class EWidgetType : uint8 
{
	None,
	MainWidget,
	CharacterSelection,
};

/**
 * 
 */
UCLASS()
class PROJECTPW_API UMasterWidget : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

private:

	void HideAllWidget();

	void ShowWidget(EWidgetType TargetWidgetType);

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UMainWidget* MainWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UCharacterSelectionWidget* CharacterSelectionWidget;

private:

	UPROPERTY(Transient)
	TArray<class UPWUserWidget*> WidgetList;
};
