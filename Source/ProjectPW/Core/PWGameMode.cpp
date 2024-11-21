// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWCommanderPointActor.h"
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


void APWGameMode::BeginPlay()
{
	Super::BeginPlay();

	BindEvents();
}

void APWGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	//한번만 바인딩 하기 위해서 서버 로컬 컨트롤러에서만
	if (NewPlayer->IsLocalPlayerController() == true)
	{
		InitialPossessedCount = 0;
		UPWEventManager* EventManager = UPWEventManager::Get(this);
		if (IsValid(EventManager) == true)
		{
			EventManager->InitialPossessDelegate.AddUObject(this, &APWGameMode::OnInitialPossess);
		}
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

void APWGameMode::OnInitialPossess(class APWPlayerController* PWPlayerController)
{
	InitialPossessedCount++;

	//둘 다 폰이 생겨 빙의되면 게임 시작
	if (InitialPossessedCount >= 2)
	{
		UPWEventManager* EventManager = UPWEventManager::Get(this);
		if (IsValid(EventManager) == true)
		{
			EventManager->InitialPossessDelegate.RemoveAll(this);
		}

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

	if (Player->HasAuthority() == false)
	{
		return;
	}

	//서버에 있는 로컬 컨트롤러면 랜덤 배분
	bool bIsRedTeam = false;
	if (Player->IsLocalPlayerController() == true)
	{
		bIsRedTeam = FMath::RandBool();
	}
	//서버에 있는 클라 컨트롤러면 서버 반대로
	else
	{
		APWPlayerController* ServerPlayerController = UPWGameplayStatics::GetLocalPlayerController(this);
		if (IsValid(ServerPlayerController) == true)
		{
			bIsRedTeam = !(ServerPlayerController->GetTeamSide() == ETeamSide::Red);
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

void APWGameMode::ReadyToStart()
{
	//Set Team Side
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