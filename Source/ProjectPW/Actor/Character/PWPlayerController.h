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

	ETeamSide GetTeamSide() const;

	//턴 변경됨
	UFUNCTION(Client, Reliable)
	void SC_ChangeTurn(bool bMyTurn);
	void SC_ChangeTurn_Implementation(bool bMyTurn);

	//게임 오버
	UFUNCTION(Client, Reliable)
	void SC_GameOver(bool bWon);
	void SC_GameOver_Implementation(bool bWon);

	UFUNCTION(Server, Reliable)
	void CS_LoadPrevSnapshot();
	void CS_LoadPrevSnapshot_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void SM_ApplyTurnActivePoint(float InTurnActivePoint);
	void SM_ApplyTurnActivePoint_Implementation(float InTurnActivePoint);

	UFUNCTION(Server, Reliable)
	void CS_NextTurn();
	void CS_NextTurn_Implementation();

	void LP_SelectCharacter(int32 SelectNum);

private:

	UFUNCTION(Server, Reliable)
	void CS_Possess(APawn* PossessablePawn, float CurrentTurnActivePointForSnapshot);
	void CS_Possess_Implementation(APawn* PossessablePawn, float CurrentTurnActivePointForSnapshot);

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
