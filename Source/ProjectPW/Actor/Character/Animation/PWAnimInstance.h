// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game

//Default
#include "PWAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EAnimState : uint8
{
	IdleWalkRun,
	JumpStart,
	Jumping,
	JumpEnd,
};

enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API UPWAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	virtual void NativeBeginPlay() override;

private:

	void InitializeAnimInstance();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* AnimSequence_JumpStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* AnimSequence_Jumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* AnimSequence_JumpEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UBlendSpace* BlendSpace;

private:

	UPROPERTY(Transient)
	ECharacterType CurrentCharacterType;

	UPROPERTY(Transient)
	class UPWAnimDataAsset* PWAnimDataAsset;
};
