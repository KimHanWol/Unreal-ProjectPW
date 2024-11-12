// LINK


//Engine
#include "Kismet/GameplayStatics.h"

//Game
#include "ProjectPW/Actor/PWPlayerStart.h"

//Default
#include "PWGameMode.h"

AActor* APWGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// 현재 플레이어의 수에 따라서 플레이어의 Index 설정 
	int32 PlayerNum = GetNumPlayers();
	return GetSpawnPoints(PlayerNum - 1);
}

AActor* APWGameMode::GetSpawnPoints(int32 PlayerIndex) const
{
	TArray<AActor*> PlayerStartList;
	UGameplayStatics::GetAllActorsOfClass(this, APWPlayerStart::StaticClass(), PlayerStartList);

	for (AActor* PlayerStartActor : PlayerStartList)
	{
		const APWPlayerStart* PlayerStart = Cast<APWPlayerStart>(PlayerStartActor);
		if (IsValid(PlayerStart) == true)
		{
			if (PlayerIndex == PlayerStart->GetPlayerIndex()) 
			{
				return PlayerStartActor;
			}
		}
	}

	return nullptr;
}