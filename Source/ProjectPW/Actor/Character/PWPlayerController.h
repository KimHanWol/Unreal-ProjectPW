// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerController.h"

//Default
#include "PWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPW_API APWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

public:

	UFUNCTION(Client, Reliable)
	void SC_ChangeTurn(bool bMyTurn);
	void SC_ChangeTurn_Implementation(bool bMyTurn);

public:

	DECLARE_MULTICAST_DELEGATE(FPlayerStateLoaded)
	FPlayerStateLoaded PlayerStateLoadedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

private:

	UPROPERTY(EditDefaultsOnly)
	class UMasterWidget* MasterWidget;

	UPROPERTY(transient)
	bool IsMyTurn = false;

};
