// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "UI/PWUserWidget.h"

//Default
#include "MainWidget_CharacterButton.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API UMainWidget_CharacterButton : public UPWUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:

	void InitializeCharacterButton(class APWPlayerCharacter* InOwnedPlayerCharacter);
	virtual FReply NativeOnMouseButtonDown( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;

protected:

	virtual void BindEvents() override;
	virtual void UnbindEvents() override;

	virtual void InvalidateWidget() override;

private:

	void OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth);

public:

	DECLARE_MULTICAST_DELEGATE(FButtonPressed)
	FButtonPressed ButtonPressedDelegate;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 ButtonIndex = 0;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Num;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UImage* Image_Portrait;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	class UProgressBar* PBar_Health;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Name;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Damage;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_Health;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* Text_HealAmount;

private:

	UPROPERTY(Transient)
	FName CharacterKey = NAME_None;

	UPROPERTY(Transient)
	class APWPlayerCharacter* OwnedPlayerCharacter;
};
