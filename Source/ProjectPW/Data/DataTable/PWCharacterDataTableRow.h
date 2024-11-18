// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/DataTable.h"

//Default
#include "PWCharacterDataTableRow.generated.h"


USTRUCT(BlueprintType)
struct PROJECTPW_API FPWCharacterDataTableRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UImage> Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float HealAmount = 50.f;
};
