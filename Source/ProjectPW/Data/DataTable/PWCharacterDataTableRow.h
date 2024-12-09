﻿// LINK

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D> Portrait;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USkeletalMesh> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class APWEquipmentActorBase> EquipmentActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float Damage = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	float HealAmount = 50.f;
};
