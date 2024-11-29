// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "GameFramework/Actor.h"
#include "Data/PWGameStruct.h"

//Default
#include "PWVolumeActorBase.generated.h"

UCLASS(Blueprintable)
class PROJECTPW_API APWVolumeActorBase : public AActor
{
	GENERATED_BODY()
	
protected:

	APWVolumeActorBase(const FObjectInitializer& ObjInit = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	virtual void Execute_Internal(AActor* OverlappedActor) {};

public:

	//이번 라운드 마무리 시 실행
	void ExecuteByRound();

private:

	UFUNCTION()
	void OnVolumeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* BoundMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	FVector DefaultActorScale = FVector(5.f);

	UPROPERTY(EditDefaultsOnly)
	float DefaultMeshHeight = -250.f;

	// 몇 턴간 이 볼륨이 지속될 것 인지 (Infinity = -1)
	UPROPERTY(EditAnywhere)
	int32 VolumeActorLifeTimeByTurn = -1;

	// 볼륨 안에 들어갈 때 바로 효과를 받는 지
	UPROPERTY(EditAnywhere)
	bool bExecuteImmediatelyWhenOverlap;

	// 실행 되면 바로 제거할 것인지
	UPROPERTY(EditAnywhere)
	bool bDestroyWhenExecute = false;

};
