// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/GameModeBase.h"

//Default
#include "PWGameMode_MainMenu.generated.h"


UCLASS()
class PROJECTPW_API APWGameMode_MainMenu : public AGameModeBase
{
	GENERATED_BODY()
	

protected:

	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

private:

	void OnCreateSessionComplete(bool bWasSuccessful);
	void OnDestroySessionComplete(bool bWasSuccessful);

	void TravelInGameLevel();

private:

	UPROPERTY(Transient)
	bool bSessionCreated = false;

};
