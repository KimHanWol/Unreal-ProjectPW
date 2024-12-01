// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "CharacterSelectionWidget.generated.h"

UCLASS()
class PROJECTPW_API UCharacterSelectionWidget : public UPWUserWidget
{
	GENERATED_BODY()

public:

	void InitializeCharacterSelectionWidget();

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharacterSelectionButton> CharacterSelectionButtonClass;

	UPROPERTY(EditDefaultsOnly)
	float CharacterSelectionButtonSpaceOffset = 20.f;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UVerticalBox* VBox_CharacterClass;
};
