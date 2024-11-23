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

	void ChangeTurn(bool bMyTurn);

	ETeamSide GetTeamSide() const;

	UFUNCTION(Client, Reliable)
	void SC_GameOver(bool bWon);
	void SC_GameOver_Implementation(bool bWon);

	UFUNCTION(NetMulticast, Reliable)
	void SM_OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth);
	void SM_OnHealthChanged_Implementation(AActor* TargetActor, float MaxHealth, float CurrentHealth);

	void SelectCharacter(int32 SelectNum);

private:

	void SS_ChangeTurn(bool bMyTurn);

	UFUNCTION(Client, Reliable)
	void SC_ChangeTurn(bool bMyTurn);
	void SC_ChangeTurn_Implementation(bool bMyTurn);

	UFUNCTION(Server, Reliable)
	void CS_ChangeTurn(bool bMyTurn);
	void CS_ChangeTurn_Implementation(bool bMyTurn);

	UFUNCTION(Client, Reliable)
	void SC_ChangeInputEnabled(bool bEnableCommander, bool bEnableCharacter);
	void SC_ChangeInputEnabled_Implementation(bool bEnableCommander, bool bEnableCharacter);

	UFUNCTION(Client, Reliable)
	void SC_OnPossess(APawn* PossessedPawn, bool bIsCommander);
	void SC_OnPossess_Implementation(APawn* PossessedPawn, bool bIsCommander);

	UFUNCTION(Server, Reliable)
	void CS_SelectCharacter(int32 SelectNum);
	void CS_SelectCharacter_Implementation(int32 SelectNum);

	void UpdateTurnData();

public:

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FInitialPossessed, APWPlayerController* SelfPlayerController)
	FInitialPossessed InitialPossessedDelegate;

private:

	UPROPERTY(EditDefaultsOnly)
	class UMasterWidget* MasterWidget;

	//Component
	UPROPERTY(transient)
	class UPWPlayerInputComponent* PlayerInputComponent;

	UPROPERTY(transient)
	bool bIsMyTurn = false;

	UPROPERTY(transient)
	bool bTurnDataDirty = false;
};
