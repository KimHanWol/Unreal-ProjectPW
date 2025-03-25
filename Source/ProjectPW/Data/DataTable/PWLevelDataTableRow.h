// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/DataTable.h"

//Default
#include "PWLevelDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct PROJECTPW_API FPWLevelDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D> LevelImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UWorld> Level;

};
