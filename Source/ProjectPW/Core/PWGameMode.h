// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/GameModeBase.h"

//Default
#include "PWGameMode.generated.h"

enum class ETeamSide : uint8;

UCLASS()
class PROJECTPW_API APWGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:

	bool IsAllPlayerReadyToStart() const;

protected:

	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* ExitPlayer) override;
	virtual void PostLogin(class APlayerController* NewPlayer) override;

private:

	void ReadyToStart();
	void StartGame(); // 캐릭터 선택
	void TryGameReady();
	void PlayBattle(); // 게임 시작

	void TransformCommanderPawns();

	void CheckGameOver();
	void OnEntireGameOver(); // 게임 종료
	void OnMatchMakingSuccess();
	void OnClientTeamSideInitialized();
	void OnTeamCharacterAllSpawned(const class APWPlayerController* TargetPlayerController, const TArray<class APWPlayerCharacter*>& TeamCharacterList);
	void OnInitialPossess(class APWPlayerController* SelfPlayerController);

	AActor* GetCommanderPointActor(ETeamSide TeamSide) const;

private:

	//처음 폰에 빙의된 클라이언트 수
	UPROPERTY(Transient)
	int32 InitialPossessedCount = 0;

	UPROPERTY(Transient)
	int32 TeamInitializedPlayerCount = 0;

	UPROPERTY(Transient)
	int32 TotalPlayerCount;

	//각자 스폰을 마치고 게임 시작 준비가 된 컨트롤러
	UPROPERTY(Transient)
	TMap<class APWPlayerController*, bool> PlayerControllerMap;
};
