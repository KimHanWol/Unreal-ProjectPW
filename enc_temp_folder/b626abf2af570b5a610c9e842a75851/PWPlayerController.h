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
	
protected:
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AcknowledgePossession(class APawn* P) override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
public:

	UFUNCTION(Client, Reliable)
	void SC_ChangeTurn(bool bMyTurn);
	void SC_ChangeTurn_Implementation(bool bMyTurn);

	void SelectCharacter(int32 SelectNum);

	void SetTeamSide(ETeamSide NewTeamSide);
	ETeamSide GetTeamSide() const { return TeamSide; }

private:

	UFUNCTION()
	void OnRep_TeamSide();

	void TryCreateInputHandler();

public:

	DECLARE_MULTICAST_DELEGATE(FPlayerStateLoaded)
	FPlayerStateLoaded PlayerStateLoadedDelegate;

	DECLARE_MULTICAST_DELEGATE_OneParam(FTurnChanged, bool bIsMyTurn)
	FTurnChanged TurnChangedDelegate;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	class UInputMappingContext* DefaultContext;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCommanderInputHandler> CommanderInputHandlerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCharacterInputHandler> CharacterInputHandlerClass;

	UPROPERTY(EditDefaultsOnly)
	class UMasterWidget* MasterWidget;

	UPROPERTY(ReplicatedUsing = OnRep_TeamSide)
	ETeamSide TeamSide;

	UPROPERTY(transient)
	class UCommanderInputHandler* CommanderInputHandler;

	UPROPERTY(transient)
	class UCharacterInputHandler* CharacterInputHandler;

	UPROPERTY(transient)
	class APawn* CommanderPawn;

	UPROPERTY(transient)
	TArray<class APWPlayerCharacter*> PossessableCharacterList;

	UPROPERTY(transient)
	bool IsMyTurn = false;

};
