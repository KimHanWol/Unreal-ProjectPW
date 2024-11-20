// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWPlayerStart.h"
#include "Data/PWGameEnum.h"
#include "Helper/PWGameplayStatics.h"
#include "PWGameState.h"
#include "PWPlayerState.h"
#include "PWEventManager.h"

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

	BindEvents();
}

AActor* APWGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	ETeamSide PlayerTeamSide = ChooseTeamSide(Player);
	
	return GetSpawnPoints(PlayerTeamSide);
}

void APWGameMode::BeginPlay()
{
	Super::BeginPlay();

	LoadCharacter();
}

void APWGameMode::LoadCharacter()
{
	for (TActorIterator<APWPlayerCharacter> PlayerCharacter(GetWorld()); PlayerCharacter; ++PlayerCharacter)
	{
		if (IsValid(*PlayerCharacter) == false)
		{
			continue;
		}

		AliveCharacterMap.Add(TTuple<APWPlayerCharacter*, ETeamSide>(*PlayerCharacter, PlayerCharacter->GetTeamSide()));
	}
}

void APWGameMode::BindEvents()
{
	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == false)
	{
		ensure(false);
		return;
	}

	PWEventManager->CharacterDeadDelegate.AddUObject(this, &APWGameMode::OnCharcterDead);
}

void APWGameMode::OnCharcterDead(APWPlayerCharacter* DeadCharacter)
{
	if (AliveCharacterMap.Contains(DeadCharacter) == true)
	{
		AliveCharacterMap.Remove(DeadCharacter);
		CheckGameOver();
	}
}

void APWGameMode::CheckGameOver()
{
	int32 RedTeamAliveCount = 0;
	int32 BlueTeamAliveCount = 0;
	for (TTuple<class APWPlayerCharacter*, ETeamSide> AliveCharacterData : AliveCharacterMap)
	{
		if (AliveCharacterData.Value == ETeamSide::Red)
		{
			++RedTeamAliveCount;
		}
		else
		{
			++BlueTeamAliveCount;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Score : Red : %d, Blue : %d"), RedTeamAliveCount, BlueTeamAliveCount);

	//Red Win
	if (RedTeamAliveCount == 0 || BlueTeamAliveCount == 0)
	{
		int32 PlayerCount = UPWGameplayStatics::GetNumPlayerControllers(this);
		for (int32 i = 0; i < PlayerCount; i++)
		{
			APWPlayerController* PlayerController = Cast<APWPlayerController>(UPWGameplayStatics::GetPlayerController(this, i));
			if (IsValid(PlayerController) == true)
			{
				//Red Win
				if(BlueTeamAliveCount == 0)
				{
					if(PlayerController->GetTeamSide() == ETeamSide::Red)
					{
						PlayerController->SC_GameOver(true);
					}
					else
					{
						PlayerController->SC_GameOver(false);
					}
				}
				//Blue Win
				else
				{
					if(PlayerController->GetTeamSide() == ETeamSide::Blue)
					{
						PlayerController->SC_GameOver(true);
					}
					else
					{
						PlayerController->SC_GameOver(false);
					}
				}
			}
		}

		OnGameOver();
	}
}

void APWGameMode::OnGameOver()
{
	//Return to Commander?
	//Restart?
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
		PWPlayerController->CS_SetTeamSide(PlayerTeamSide);
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
		PlayerControllerInTurn->ChangeTurn(true);
	}
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
		PlayerControllerInTurn->ChangeTurn(false);
	}

	PWGameState->NextTurn();
}