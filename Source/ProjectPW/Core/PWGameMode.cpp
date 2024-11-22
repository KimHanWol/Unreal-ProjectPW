// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWCommanderPointActor.h"
#include "Helper/PWGameplayStatics.h"
#include "PWGameState.h"
#include "PWPlayerState.h"
#include "PWEventManager.h"

void APWGameMode::StartPlay()
{
	Super::StartPlay();
	BindEvents();
}

void APWGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APWPlayerController* PWPlayerController = Cast<APWPlayerController>(NewPlayer);
	if (IsValid(PWPlayerController) == true)
	{
		PWPlayerController->InitialPossessedDelegate.AddUObject(this, &APWGameMode::OnInitialPossess);
	}

	if (GetNumPlayers() == 2)
	{
		ReadyToStart();
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
	CheckGameOver();
}

void APWGameMode::OnInitialPossess(APWPlayerController* SelfPlayerController)
{
	if (ensure(IsValid(SelfPlayerController) == true))
	{
		SelfPlayerController->InitialPossessedDelegate.RemoveAll(this);
	}

	InitialPossessedCount++;

	//둘 다 폰이 생겨 빙의되면 게임 시작
	if (InitialPossessedCount >= 2)
	{
		StartGame();
	}
}

void APWGameMode::CheckGameOver()
{
	if (IsValid(RedTeamPlayerController) == false)
	{
		ensure(false);
		return;
	}

	if (IsValid(BlueTeamPlayerController) == false)
	{
		ensure(false);
		return;
	}

	int32 RedTeamCharacterNum = 0;
	APWPlayerState* RedPlayerState = RedTeamPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(RedPlayerState) == true)
	{
		RedTeamCharacterNum = RedPlayerState->GetAliveTeamCharacterNum();
	}

	int32 BlueTeamCharacterNum = 0;
	APWPlayerState* BluePlayerState = BlueTeamPlayerController->GetPlayerState<APWPlayerState>();
	if (IsValid(BluePlayerState) == true)
	{
		BlueTeamCharacterNum = BluePlayerState->GetAliveTeamCharacterNum();
	}

	UE_LOG(LogTemp, Warning, TEXT("Current Score : Red : %d, Blue : %d"), RedTeamCharacterNum, BlueTeamCharacterNum);

	bool bGameOver = false;
	//Blue team win
	if (RedTeamCharacterNum <= 0)
	{
		RedTeamPlayerController->SC_GameOver(false);
		BlueTeamPlayerController->SC_GameOver(true);

		bGameOver = true;
	}

	//Red team win
	else if (BlueTeamCharacterNum <= 0)
	{
		RedTeamPlayerController->SC_GameOver(true);
		BlueTeamPlayerController->SC_GameOver(false);

		bGameOver = true;
	}

	//GameOver
	if (bGameOver == true)
	{
		OnGameOver();
	}
}

void APWGameMode::OnGameOver()
{
	//Return to Commander?
	//Restart?
}

void APWGameMode::ChooseTeamSide(AController* Player)
{
	if (IsValid(Player) == false)
	{
		ensure(false);
		return;
	}

	bool bIsRedTeam = false;
	//처음 입장했으면 랜덤
	if (IsValid(RedTeamPlayerController) == false && IsValid(BlueTeamPlayerController) == false)
	{
		bIsRedTeam = FMath::RandBool();
	}
	//다음 입장했으면 처음거 반대
	else
	{
		if (IsValid(RedTeamPlayerController) == true)
		{
			bIsRedTeam = false;
		}
		else
		{
			bIsRedTeam = true;
		}
	}

	ETeamSide PlayerTeamSide = bIsRedTeam == true ? ETeamSide::Red : ETeamSide::Blue;
	APWPlayerState* PWPlayerState = Player->GetPlayerState<APWPlayerState>();
	if (IsValid(PWPlayerState) == true)
	{
		PWPlayerState->SS_InitializePlayerState(PlayerTeamSide);
	}

	if (bIsRedTeam == true)
	{
		RedTeamPlayerController = Cast<APWPlayerController>(Player);
	}
	else
	{
		BlueTeamPlayerController = Cast<APWPlayerController>(Player);
	}
}

AActor* APWGameMode::GetCommanderPointActor(ETeamSide TeamSide) const
{
	TArray<AActor*> CommanderPointActorList;
	UGameplayStatics::GetAllActorsOfClass(this, APWCommanderPointActor::StaticClass(), CommanderPointActorList);

	for (AActor* InCommanderPointActor : CommanderPointActorList)
	{
		const APWCommanderPointActor* CommanderPointActor = Cast<APWCommanderPointActor>(InCommanderPointActor);
		if (IsValid(CommanderPointActor) == true)
		{
			if (TeamSide == CommanderPointActor->GetTeamSide()) 
			{
				return InCommanderPointActor;
			}
		}
	}

	return nullptr;
}

void APWGameMode::TransformCommanderPawns()
{
	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	for (APWPlayerController* PlayerController : PlayerControllerList)
	{
		AActor* CommanderPointActor = GetCommanderPointActor(PlayerController->GetTeamSide());
		APawn* CommanderPawn = PlayerController->GetPawn();
		if (IsValid(CommanderPointActor) == true && IsValid(CommanderPawn) == true)
		{
			CommanderPawn->SetActorTransform(CommanderPointActor->GetActorTransform());	
		}
	}
}

int32 APWGameMode::GetCurrentPlayerTurn()
{
	int32 CurrentPlayerTurn = 0;
	APWGameState* PWGameState = Cast<APWGameState>(UGameplayStatics::GetGameState(this));
	if (ensure(IsValid(PWGameState) == true))
	{
		CurrentPlayerTurn = PWGameState->GetCurrentPlayerTurn();
	}

	return CurrentPlayerTurn;
}

void APWGameMode::ReadyToStart()
{
	//Set team side
	int32 ControllerNum = UPWGameplayStatics::GetNumPlayerControllers(this);
	for (int32 i = 0; i < ControllerNum; i++)
	{
		ChooseTeamSide(UPWGameplayStatics::GetPlayerController(this, i));
	}
}

void APWGameMode::StartGame()
{
	//Move player to commander position
	TransformCommanderPawns();

	//Turn setting
	APWPlayerController* PlayerControllerInTurn = Cast<APWPlayerController>(UPWGameplayStatics::GetPlayerController(this, GetCurrentPlayerTurn()));
	if (IsValid(PlayerControllerInTurn) == true)
	{
		PlayerControllerInTurn->ChangeTurn(true);
	}
}

void APWGameMode::TextTurn()
{
	APWGameState* PWGameState = Cast<APWGameState>(UGameplayStatics::GetGameState(this));
	if (IsValid(PWGameState) == true)
	{
		PWGameState->NextTurn();
	}

	APWPlayerController* PlayerControllerInTurn = Cast<APWPlayerController>(UPWGameplayStatics::GetPlayerController(this, GetCurrentPlayerTurn()));
	if (IsValid(PlayerControllerInTurn) == true)
	{
		PlayerControllerInTurn->ChangeTurn(false);
	}
}
