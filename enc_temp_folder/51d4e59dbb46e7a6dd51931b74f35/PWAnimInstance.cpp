// LINK

//Default
#include "PWAnimInstance.h"

//Engine

//Game
#include "Actor/Character/PWPlayerCharacter.h"
#include "Data/DataAsset/PWAnimDataAsset.h"
#include "Data/DataTable/PWCharacterDataTableRow.h"

void UPWAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	InitializeAnimInstance();
}

void UPWAnimInstance::InitializeAnimInstance()
{
	const APWPlayerCharacter* OwnerCharacter = Cast<APWPlayerCharacter>(TryGetPawnOwner());
	if (IsValid(OwnerCharacter) == false)
	{
		ensure(false);
		return;
	}

	const FPWCharacterDataTableRow* CharacterData = OwnerCharacter->GetCharacterData();
	if (CharacterData == nullptr)
	{
		ensure(false);
		return;
	}

	CurrentCharacterType = CharacterData->CharacterType;

	PWAnimDataAsset = const_cast<UPWAnimDataAsset*>(UPWAnimDataAsset::Get(this, CurrentCharacterType));
	if (IsValid(PWAnimDataAsset) == false)
	{
		ensure(false);
		return;
	}

	TSoftObjectPtr<UAnimationAsset> JumpStartAnim = PWAnimDataAsset->GetAnimation(this, CurrentCharacterType, EAnimationType::JumpStart);
	if (ensure(JumpStartAnim.IsNull() == false))
	{
		AnimSequence_JumpStart = Cast<UAnimSequence>(JumpStartAnim.LoadSynchronous());
	}

	TSoftObjectPtr<UAnimationAsset> JumpingAnim = PWAnimDataAsset->GetAnimation(this, CurrentCharacterType, EAnimationType::Jumping);
	if (ensure(JumpingAnim.IsNull() == false))
	{
		AnimSequence_Jumping = Cast<UAnimSequence>(JumpingAnim.LoadSynchronous());
	}

	TSoftObjectPtr<UAnimationAsset> JumpEndAnim = PWAnimDataAsset->GetAnimation(this, CurrentCharacterType, EAnimationType::JumpEnd);
	if (ensure(JumpEndAnim.IsNull() == false))
	{
		AnimSequence_JumpEnd = Cast<UAnimSequence>(JumpEndAnim.LoadSynchronous());
	}

	TSoftObjectPtr<UBlendSpace> NewBlendSpace = PWAnimDataAsset->GetBlendSpace(this, CurrentCharacterType);
	if (ensure(NewBlendSpace.IsNull() == false))
	{
		BlendSpace = NewBlendSpace.LoadSynchronous();
	}
}
