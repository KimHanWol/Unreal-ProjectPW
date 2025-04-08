// LINK

#pragma once

//Engine
#include "CoreMinimal.h"

//Game
#include "Engine/DataTable.h"

//Default
#include "PWCharacterDataTableRow.generated.h"

enum class ECharacterType : uint8;

USTRUCT(BlueprintType)
struct PROJECTPW_API FPWCharacterDataTableRow: public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftObjectPtr<class UTexture2D> Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftObjectPtr<class UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	TSoftObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FName EquipmentKey;

	//TODO: AsyncLoad 할 수 있는지 확인
	//내부 에셋들 AsyncLoad 하려고 하드 레퍼런스로 가져옴
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animaiton")
	class UPWAnimDataAsset* AnimDataAsset;
};
