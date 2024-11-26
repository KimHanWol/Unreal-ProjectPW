// LINK

//Default
#include "PWGameSetting.h"

//Engine

//Game
#include "Core/PWGameInstance.h"

const UPWGameSetting* UPWGameSetting::Get(const UObject* WorldContextObj)
{
	UPWGameInstance* PWGameInst = UPWGameInstance::Get(WorldContextObj);
	if (ensure(IsValid(PWGameInst) == true))
	{
		return PWGameInst->GetGameSetting();
	}

	return nullptr;
}
