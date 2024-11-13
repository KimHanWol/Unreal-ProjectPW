// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "Kismet/GameplayStatics.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWPlayerStart.h"
#include "Data/PWGameEnum.h"
#include "PWGameState.h"
#include "Helper/PWGameplayStatics.h"

//현재 차례인 플레이어 컨트롤러 반환
#define GET_PLAYER_INTURN(InWorldContext, InCurrentPlayersTurn) \
	(Cast<APWPlayerController>(UGameplayStatics::GetPlayerController(InWorldContext->GetWorld(), InCurrentPlayersTurn)))

//다음 차례인 플레이어 컨트롤러 반환
#define GET_PLAYER_NOTTURN(InWorldContext, InCurrentPlayersTurn) \
	(Cast<APWPlayerController>(UGameplayStatics::GetPlayerController(InWorldContext->GetWorld(), 1 - InCurrentPlayersTurn)))


void APWGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	if (GetNumPlayers() == 2)
	{
		StartGame();
	}
}

AActor* APWGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	ETeamSide PlayerTeamSide = ChooseTeamSide(Player);
	
	return GetSpawnPoints(PlayerTeamSide);
}

ETeamSide APWGameMode::ChooseTeamSide(AController* Player)
{
	//서버 컨트롤러면 랜덤 배분
	bool bIsRedTeam = false;
	if (Player->IsLocalPlayerController() == true)
	{
		bIsRedTeam = FMath::RandBool();
	}
	//클라 컨트롤러면 서버 반대로
	else
	{
		APWPlayerController* ServerPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
		if (IsValid(ServerPlayerController) == true)
		{
			bIsRedTeam = !(ServerPlayerController->GetTeamSide() == ETeamSide::Red);
		}
	}

	ETeamSide PlayerTeamSide = bIsRedTeam == true ? ETeamSide::Red : ETeamSide::Blue;

	APWPlayerController* PWPlayerController = Cast<APWPlayerController>(Player);
	if (IsValid(PWPlayerController) == true)
	{
		PWPlayerController->SetTeamSide(PlayerTeamSide);
	}

	return PlayerTeamSide;
}

AActor* APWGameMode::GetSpawnPoints(ETeamSide TeamSide) const
{
	TArray<AActor*> PlayerStartList;
	UGameplayStatics::GetAllActorsOfClass(this, APWPlayerStart::StaticClass(), PlayerStartList);

	for (AActor* PlayerStartActor : PlayerStartList)
	{
		const APWPlayerStart* PlayerStart = Cast<APWPlayerStart>(PlayerStartActor);
		if (IsValid(PlayerStart) == true)
		{
			if (TeamSide == PlayerStart->GetTeamSide()) 
			{
				return PlayerStartActor;
			}
		}
	}

	return nullptr;
}

void APWGameMode::StartGame()
{
	//Turn Setting
	APWPlayerController* PlayerControllerInTurn = GET_PLAYER_INTURN(this, 0);
	if (IsValid(PlayerControllerInTurn) == true)
	{
		PlayerControllerInTurn->SC_ChangeTurn(true);
	}

	APWPlayerController* PlayerControllerNotTurn = GET_PLAYER_NOTTURN(this, 0);
	if (IsValid(PlayerControllerNotTurn) == true)
	{
		PlayerControllerNotTurn->SC_ChangeTurn(false);
	}

	UE_LOG(LogTemp, Log, TEXT("Turn Started : 0 Turn, Player 0"));
}

void APWGameMode::TextTurn()
{
	APWGameState* PWGameState = Cast<APWGameState>(UGameplayStatics::GetGameState(this));
	if (IsValid(PWGameState) == false)
	{
		ensure(false);
		return;
	}

	int32 CurrentPlayersTurn = PWGameState->GetCurrentPlayerTurn();

	APWPlayerController* PlayerControllerInTurn = GET_PLAYER_INTURN(this, CurrentPlayersTurn);
	if (IsValid(PlayerControllerInTurn) == true)
	{
		PlayerControllerInTurn->SC_ChangeTurn(false);
	}

	APWPlayerController* PlayerControllerNotTurn = GET_PLAYER_NOTTURN(this, CurrentPlayersTurn);
	if (IsValid(PlayerControllerNotTurn) == true)
	{
		PlayerControllerNotTurn->SC_ChangeTurn(true);
	}

	PWGameState->NextTurn();
}