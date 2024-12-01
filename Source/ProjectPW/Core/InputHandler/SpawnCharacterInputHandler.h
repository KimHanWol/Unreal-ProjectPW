// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Core/InputHandler/InputHandlerBase.h"

//Default
#include "Data/PWGameEnum.h"
#include "SpawnCharacterInputHandler.generated.h"

enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API USpawnCharacterInputHandler : public UInputHandlerBase
{
	GENERATED_BODY()

public:

	virtual void SetupKeyBindings(class APWPlayerController* InPWPlayerController) override;

private:

	void Select_1(const struct FInputActionValue& Value);
	void Select_2(const struct FInputActionValue& Value);
	void Select_3(const struct FInputActionValue& Value);

	void TrySpawn(const struct FInputActionValue& Value);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SpawnAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectCharacter_1;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectCharacter_2;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputAction* SelectCharacter_3;

private:

	UPROPERTY(Transient)
	ECharacterType SelectedCharacterType = ECharacterType::Soldier;

	UPROPERTY(Transient)
	class APWPlayerController* PWPlayerController;
};
