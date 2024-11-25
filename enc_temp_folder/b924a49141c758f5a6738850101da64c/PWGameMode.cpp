// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/RandomShuffle.h"

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWCommanderPointActor.h"
#include "Core/Subsystem/PWTurnManageSubsystem.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Helper/PWGameplayStatics.h"
#include "PWGameState.h"
#include "PWPlayerState.h"
#include "PWEventManager.h"


void APWGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APWPlayerController* PWPlayerController = Cast<APWPlayerController>(NewPlayer);
	if (IsValid(PWPlayerController) == true)
	{
		PWPlayerController->InitialPossessedDelegate.AddUObject(this, &APWGameMode::OnInitialPossess);
	}

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return;
	}
	
	if (GetNumPlayers() == PWGameSetting->PlayerCount)
	{
		ReadyToStart();
		TotalPlayerCount = PWGameSetting->PlayerCount;
	}
}

void APWGameMode::OnInitialPossess(APWPlayerController* SelfPlayerController)
{
	if (ensure(IsValid(SelfPlayerController) == true))
	{
		SelfPlayerController->InitialPossessedDelegate.RemoveAll(this);
	}

	InitialPossessedCount++;

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return;
	}

	//모두 다 폰이 생겨 빙의되면 게임 시작
	if (InitialPossessedCount >= PWGameSetting->PlayerCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d"), InitialPossessedCount);
		StartGame();
	}
}

void APWGameMode::CheckGameOver()
{
	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	int32 LosePlayerCount = 0;

	FString LogString;
	LogString += TEXT("=====CHECK GAME OVER=====\n");

	APWPlayerController* WinCandidatePlayerController = PlayerControllerList[0];
	for (APWPlayerController* PWPlayerController : PlayerControllerList)
	{
		if (IsValid(PWPlayerController) == false)
		{
			ensure(false);
			continue;
		}

		APWPlayerState* PWPlayerState = PWPlayerController->GetPlayerState<APWPlayerState>();
		if (IsValid(PWPlayerState) == true)
		{
			//Lose
			if (PWPlayerState->GetAliveTeamCharacterNum() == 0)
			{
				PWPlayerController->SM_GameOver(PWPlayerController, true);
				LosePlayerCount++;
				PWPlayerState->GameOver();
			}
			else
			{
				WinCandidatePlayerController = PWPlayerController;
			}
			LogString += UPWGameplayStatics::ConvertEnumToString(this, PWPlayerState->GetTeamSide()) + TEXT(" ") + FString::FromInt(PWPlayerState->GetAliveTeamCharacterNum()) + TEXT(" ") + TEXT("Alived \n");
		}
	}

	//Win
	if (LosePlayerCount == PlayerControllerList.Num() - 1)
	{
		WinCandidatePlayerController->SM_GameOver(WinCandidatePlayerController, false);
		LogString += WinCandidatePlayerController->GetName() + TEXT(" Win\n");
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);
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
	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	Algo::RandomShuffle(PlayerControllerList);

	for (int32 i = 0; i < PlayerControllerList.Num(); i++)
	{
		APWPlayerController* PWPlayerController = PlayerControllerList[i];
		if (IsValid(PWPlayerController) == false)
		{
			ensure(false);
			continue;
		}

		APWPlayerState* PWPlayerState = PWPlayerController->GetPlayerState<APWPlayerState>();
		if (IsValid(PWPlayerState) == true)
		{
			PWPlayerState->SS_InitializePlayerState(static_cast<ETeamSide>(i+1));
		}

		UE_LOG(LogTemp, Log, TEXT("PlayerController %d is assigned to %s"), i, *UPWGameplayStatics::ConvertEnumToString(this, static_cast<ETeamSide>(i+1)));
	}
}

void APWGameMode::StartGame()
{
	//Move player to commander position
	TransformCommanderPawns();

	//Bind game over check
	APWGameState* PWGameState = GetGameState<APWGameState>();
	if (IsValid(PWGameState) == true)
	{
		PWGameState->GameStateCharacterAliveStateChangedDelegate.AddUObject(this, &APWGameMode::CheckGameOver);
	}

	//서버만 운용
	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	UPWTurnManageSubsystem* PWTurnManageSubsystem = UPWTurnManageSubsystem::Get(this);
	if (IsValid(PWTurnManageSubsystem) == true)
	{
		PWTurnManageSubsystem->StartGame(PlayerControllerList);
	}
}
