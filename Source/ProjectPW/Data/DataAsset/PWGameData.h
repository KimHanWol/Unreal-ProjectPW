// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

//Game

//Default
#include "PWGameData.generated.h"


UENUM()
enum class EGameplayEffectType : uint8
{
	None,
	AddHealth,
};

UENUM()
enum class EDataTableType : uint8
{
	None,
	Character,
};

enum class ECharacterType : uint8;

UCLASS()
class PROJECTPW_API UPWGameData: public UDataAsset
{
	GENERATED_BODY()

public:

	static const UPWGameData* Get(const UObject* WorldContextObj);

	void Initialize();

	static TSubclassOf<class UGameplayEffect> GetGameplayEffect(const UObject* WorldContextObj, EGameplayEffectType GameplayEffectType);

	template <typename TRowType>
	const TRowType* FindTableRow(EDataTableType DataTableType, const FName& KeyName) const
	{
		if (DataTableMap.Contains(DataTableType) == false)
		{
			ensureMsgf(false, TEXT("There's no data table. You may have forgotten to registor it with DataTableMap."));
			return nullptr;
		}

		TSoftObjectPtr<UDataTable> TargetDataTablePtr = DataTableMap[DataTableType];
		if (TargetDataTablePtr.IsNull() == true)
		{
			ensureMsgf(false, TEXT("Data Table is not valid."));
			return nullptr;
		}

		UDataTable* TargetDataTable = TargetDataTablePtr.LoadSynchronous();
		if (IsValid(TargetDataTable) == false)
		{
			ensure(false);
			return nullptr;
		}

		return TargetDataTable->FindRow<TRowType>(KeyName, TEXT("Find Table"));
	}

	template <typename TRowType>
	const TArray<TRowType*> GetAllTableRow(EDataTableType DataTableType) const
	{
		TArray<TRowType*> RowArray;
		if (DataTableMap.Contains(DataTableType) == false)
		{
			ensureMsgf(false, TEXT("There's no data table. You may have forgotten to registor it with DataTableMap."));
			return RowArray;
		}

		TSoftObjectPtr<UDataTable> TargetDataTablePtr = DataTableMap[DataTableType];
		if (TargetDataTablePtr.IsNull() == true)
		{
			ensureMsgf(false, TEXT("Data Table is not valid."));
			return RowArray;
		}

		UDataTable* TargetDataTable = TargetDataTablePtr.LoadSynchronous();
		if (IsValid(TargetDataTable) == false)
		{
			ensure(false);
			return RowArray;
		}

		TargetDataTable->GetAllRows<TRowType>(TEXT("GetAllTableRow"), RowArray);
		return RowArray;
	}

	static TSubclassOf<class APWVolumeActorBase> GetVolumeActorRandom(const UObject* WorldContextObj);

protected:

	static UPWGameData* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EGameplayEffectType, TSubclassOf<class UGameplayEffect>> GameplayEffectMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EDataTableType, TSoftObjectPtr<class UDataTable>> DataTableMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class APWVolumeActorBase>> VolumeActorList;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class APWPlayerCharacter> PlayerCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInstance> SpawnPreviewMI;
};
