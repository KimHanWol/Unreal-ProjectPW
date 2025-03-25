// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "CharacterSelectionWidget.generated.h"

enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API UCharacterSelectionWidget : public UPWUserWidget
{
	GENERATED_BODY()

	void virtual NativeConstruct() override;

public:

	void InitializeCharacterSelectionWidget();

private:

	void OnAllPlayerReadyToStart();
	void OnSelectedCharacterChanged(ECharacterType SelectedChracterType);\

	void PlaySelectAnimation(int32 SelectIndex);
	void PlayDeselectAnimation(int32 TargetIndex);

	FTimerHandle GetDeselectTimerHandle(int32 TargetIndex);

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharacterSelectionButton> CharacterSelectionButtonClass;

	UPROPERTY(EditDefaultsOnly)
	float CharacterSelectionButtonSpaceOffset = 20.f;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UVerticalBox* VBox_CharacterClass;

	UPROPERTY(EditDefaultsOnly)
	float SlideAnimaionDuration = 0.3f;

	UPROPERTY(Transient)
	TArray<struct FPWCharacterDataTableRow> ChildrenCharacterDataList;

	UPROPERTY(Transient)
	TArray<class UCharacterSelectionButton*> SelectionWidgetList;

	UPROPERTY(Transient)
	int32 CurrentSelectedIndex = -1;

	FTimerHandle SelectAnimationTimerHandle;
	TMap<int32, FTimerHandle> DeselectAnimationTimerHandleMap;
};
