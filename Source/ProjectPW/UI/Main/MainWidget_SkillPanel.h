// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Data/DataTable/PWCharacterDataTableRow.h"
#include "UI/PWUserWidget.h"

//Default
#include "MainWidget_SkillPanel.generated.h"

UCLASS()
class PROJECTPW_API UMainWidget_SkillPanel : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

	virtual void InvalidateWidget() override;

private:

	void OnPlayerPossessed(APawn* PossessedPawn, bool bIsCommander);

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bIsForLeftMouseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Icon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Name;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> DefaultIconTexture;

	UPROPERTY(Transient)
	ECharacterType PossessedCharacterType;
};
