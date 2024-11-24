// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/PlayerController.h"

//Default
#include "PWPlayerController.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	APWPlayerController();

protected:
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;

public:

	UFUNCTION(Client, Reliable)
	void SC_ChangeTurn(bool bMyTurn);
	void SC_ChangeTurn_Implementation(bool bMyTurn);

	UFUNCTION(Server, Reliable)
	void CS_NextTurn();
	void CS_NextTurn_Implementation();

	ETeamSide GetTeamSide() const;

	UFUNCTION(Client, Reliable)
	void SC_GameOver(bool bWon);
	void SC_GameOver_Implementation(bool bWon);

	UFUNCTION(NetMulticast, Reliable)
	void SM_OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth);
	void SM_OnHealthChanged_Implementation(AActor* TargetActor, float MaxHealth, float CurrentHealth);

	void LP_SelectCharacter(int32 SelectNum);

private:



	UFUNCTION(Server, Reliable)
	void CS_Possess(APawn* PossessablePawn);
	void CS_Possess_Implementation(APawn* PossessablePawn);

	UFUNCTION(Client, Reliable)
	void SC_ChangeInputEnabled(bool bEnableCommander, bool bEnableCharacter);
	void SC_ChangeInputEnabled_Implementation(bool bEnableCommander, bool bEnableCharacter);
	void LP_ChangeInputEnabled(bool bEnableCommander, bool bEnableCharacter);

	void UpdateTurnData();

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FInitialPossessed, APWPlayerController* SelfPlayerController)
	FInitialPossessed InitialPossessedDelegate;

private:

	UPROPERTY(EditDefaultsOnly)
	class UMasterWidget* MasterWidget;

	//Component
	UPROPERTY(EditAnywhere)
	class UPWPlayerInputComponent* PlayerInputComponent;

	UPROPERTY(transient)
	bool bIsMyTurn = false;

	UPROPERTY(transient)
	bool bTurnDataDirty = false;
};
