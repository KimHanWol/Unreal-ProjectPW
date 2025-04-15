// LINK

//Default
#include "PWGameMode.h"

//Engine
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/RandomShuffle.h"

//Game
#include "Actor/Character/PWPlayerController.h"
#include "Actor/PWCommanderPointActor.h"
#include "Core/Subsystem/PWTurnManageSubsystem.h"
#include "Data/DataAsset/PWGameSetting.h"
#include "Helper/PWGameplayStatics.h"
#include "PWEventManager.h"
#include "PWGameInstance.h"
#include "PWGameState.h"
#include "PWPlayerState.h"
#include "Subsystem/PWBGMManageSubsystem.h"
#include "Subsystem/PWSteamMatchMakingSubsystem.h"
#include "UI/MasterWidget.h"

bool APWGameMode::IsAllPlayerReadyToStart() const
{
#if WITH_EDITOR
	//
#else
	// 모든 로컬 컨트롤러에 팀이 세팅 되어야 시작 가능
	UE_LOG(LogTemp, Log, TEXT("[READY TO START] Team Initialized Player Count (%d / %d)"), TeamInitializedPlayerCount , PlayerControllerMap.Num());
	if (TeamInitializedPlayerCount < PlayerControllerMap.Num())
	{
		return false;
	}
#endif //WITH_EDITOR

	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (IsValid(PWGameSetting) == false)
	{
		ensure(false);
		return false;
	}

	//모두 다 폰이 생겨야 게임 시작 가능
	UE_LOG(LogTemp, Log, TEXT("[READY TO START] Initial Possessed Count (%d / %d)"), InitialPossessedCount, PWGameSetting->PlayerCount);
	if (InitialPossessedCount < PWGameSetting->PlayerCount)
	{
		return false;
	}

	return true;
}

void APWGameMode::BeginPlay()
{
	Super::BeginPlay();

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (ensure(IsValid(PWEventManager) == true))
	{
		PWEventManager->ClientTeamSideInitializedDelegate.AddUObject(this, &APWGameMode::OnClientTeamSideInitialized);
	}

	//Play InGame BGM
	UPWGameInstance* GameInst = UPWGameInstance::Get(this);
	if (ensure(IsValid(GameInst) == true))
	{
		UPWBGMManageSubsystem* BGMManageSubsystem = GameInst->GetPWBGMManageSubsystem();
		if (ensure(IsValid(BGMManageSubsystem) == true))
		{
			BGMManageSubsystem->PlayBGM(EBGMType::InGame);
		}
	}
}

void APWGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APWPlayerController* PWPlayerController = Cast<APWPlayerController>(NewPlayer);
	if (IsValid(PWPlayerController) == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s player post login."), *PWPlayerController->GetName());

		PWPlayerController->InitialPossessedDelegate.AddUObject(this, &APWGameMode::OnInitialPossess);

		PlayerControllerMap.Add(TTuple<APWPlayerController*, bool>(PWPlayerController, false));
	}

	// 다 들어오면 시작 준비
	const UPWGameSetting* PWGameSetting = UPWGameSetting::Get(this);
	if (ensure(IsValid(PWGameSetting) == true))
	{
		if (GetNumPlayers() == PWGameSetting->PlayerCount)
		{
			ReadyToStart();
			TotalPlayerCount = PWGameSetting->PlayerCount;
		}
	}

	bIsBattleStarted = false;
}

void APWGameMode::Logout(AController* ExitPlayer)
{
	Super::Logout(ExitPlayer);

	UPWSteamMatchMakingSubsystem* MatchMakingSubsystem = UPWSteamMatchMakingSubsystem::Get(this);
	if (IsValid(MatchMakingSubsystem) == true)
	{
		MatchMakingSubsystem->LeaveGameSession();
	}

	bIsBattleStarted = false;
}

void APWGameMode::OnInitialPossess(APWPlayerController* SelfPlayerController)
{
	if (ensure(IsValid(SelfPlayerController) == true))
	{
		SelfPlayerController->InitialPossessedDelegate.RemoveAll(this);
	}

	InitialPossessedCount++;

	UE_LOG(LogTemp, Log, TEXT("[READY TO START] %s Initial Possessed : %d"), *SelfPlayerController->GetName(), InitialPossessedCount);
	TryGameReady();
}

void APWGameMode::CheckGameOver()
{
	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	int32 LosePlayerCount = 0;

	FString LogString;
	LogString += TEXT("=====CHECK GAME OVER=====\n");

	APWPlayerController* CandidateWinnerPlayerController = PlayerControllerList[0];
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
				PWPlayerState->GameOver();
				LosePlayerCount++;
			}
			else
			{
				CandidateWinnerPlayerController = PWPlayerController;
			}
			LogString += UPWGameplayStatics::ConvertEnumToString(this, PWPlayerState->GetTeamSide()) + TEXT(" ") + FString::FromInt(PWPlayerState->GetAliveTeamCharacterNum()) + TEXT(" ") + TEXT("Alived \n");
		}
	}

	//Win
	if (LosePlayerCount == PlayerControllerList.Num() - 1)
	{
		CandidateWinnerPlayerController->SM_GameOver(CandidateWinnerPlayerController, false);
		LogString += CandidateWinnerPlayerController->GetName() + TEXT(" Win\n");
		OnEntireGameOver();
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *LogString);
}

void APWGameMode::OnEntireGameOver()
{
	APWGameState* PWGameState = GetGameState<APWGameState>();
	if (IsValid(PWGameState) == true)
	{
		PWGameState->GameStateCharacterAliveStateChangedDelegate.RemoveAll(this);
		PWGameState->OnEntireGameOver();
	}
}

void APWGameMode::OnClientTeamSideInitialized()
{
	TeamInitializedPlayerCount++;
	UE_LOG(LogTemp, Log, TEXT("[READY TO START] On Team Side Initialized : %d"), TeamInitializedPlayerCount);
	TryGameReady();
}

void APWGameMode::OnTeamCharacterAllSpawned(const APWPlayerController* TargetPlayerController, const TArray<TWeakObjectPtr<class APWPlayerCharacter>>& TargetCharacterList)
{
	if (PlayerControllerMap.Contains(TargetPlayerController) == true)
	{
		PlayerControllerMap[TargetPlayerController] = true;
	}

	bool bReadyToPlay = true;
	for (TTuple<APWPlayerController*, bool> PlayerControllerReadyData : PlayerControllerMap)
	{
		if (PlayerControllerReadyData.Value == false)
		{
			bReadyToPlay = false;
		}
	}

	if (bReadyToPlay == true)
	{
		UPWEventManager* PWEventManager = UPWEventManager::Get(this);
		if (IsValid(PWEventManager) == true)
		{
			PWEventManager->TeamCharacterAllSpawnedDelegate.RemoveAll(this);
		}

		PlayBattle();
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
			CommanderPawn->SetReplicates(true);
			CommanderPawn->SetActorTransform(CommanderPointActor->GetActorTransform());
		}
	}
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

		UE_LOG(LogTemp, Log, TEXT("[READY TO START] PlayerController %d is assigned to %s"), i, *UPWGameplayStatics::ConvertEnumToString(this, static_cast<ETeamSide>(i+1)));
	}
}

void APWGameMode::StartGame()
{
	if (bIsBattleStarted == true)
	{
		return;
	}

	bIsBattleStarted = true;

	//Move player to commander position
	TransformCommanderPawns(); 

	UPWEventManager* PWEventManager = UPWEventManager::Get(this);
	if (IsValid(PWEventManager) == true)
	{
		PWEventManager->TeamCharacterAllSpawnedDelegate.AddUObject(this, &APWGameMode::OnTeamCharacterAllSpawned);
	}

	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	for (APWPlayerController* PlayerController : PlayerControllerList)
	{
		PlayerController->SC_BattleLevelSettingFinished();
	}
}

void APWGameMode::TryGameReady()
{
	// 모든 플레이어가 준비가 되면
	if (IsAllPlayerReadyToStart() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[READY TO START] All players are not ready."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[READY TO START] All players are ready. Start game."));

	StartGame();
}

void APWGameMode::PlayBattle()
{
	//Bind game over check
	APWGameState* PWGameState = GetGameState<APWGameState>();
	if (IsValid(PWGameState) == true)
	{
		PWGameState->GameStateCharacterAliveStateChangedDelegate.AddUObject(this, &APWGameMode::CheckGameOver);
	}

	TArray<APWPlayerController*> PlayerControllerList = UPWGameplayStatics::GetAllPlayerController(this);
	for (APWPlayerController* PlayerController : PlayerControllerList)
	{
		if (IsValid(PlayerController) == true)
		{
			PlayerController->SC_SpawnCharacterFinished();
		}
	}

	//서버만 운용
	UPWTurnManageSubsystem* PWTurnManageSubsystem = UPWTurnManageSubsystem::Get(this);
	if (IsValid(PWTurnManageSubsystem) == true)
	{
		PWTurnManageSubsystem->StartGame(PlayerControllerList);
	}
}
