// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/DataTable.h"

//Default
#include "PWGameplayEffectDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct PROJECTPW_API FPWGameplayEffectDataTableRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UGameplayEffect> GameplayEffect;

};
