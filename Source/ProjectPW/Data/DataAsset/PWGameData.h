// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

//Game

//Default
#include "PWGameData.generated.h"


UENUM()
enum class EGameplayEffectType : uint8
{
	None,
	AddHealth,
	Heal,
};

UENUM()
enum class EDataTableType : uint8
{
	None,
	Character,
	Level,
	Equipment,
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
		const UDataTable* TargetDataTable = GetDataTable(DataTableType);
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
		const UDataTable* TargetDataTable = GetDataTable(DataTableType);
		if (IsValid(TargetDataTable) == false)
		{
			ensure(false);
			return RowArray;
		}

		TargetDataTable->GetAllRows<TRowType>(TEXT("GetAllTableRow"), RowArray);
		return RowArray;
	}

	static const struct FPWCharacterDataTableRow* FindCharacterTableRow(const UObject* WorldContextObj, ECharacterType CharacterType);
	static const struct FPWLevelDataTableRow* FindLevelTableRow(const UObject* WorldContextObj, FName LevelDataKey);
	static const struct FPWEquipmentDataTableRow* FindEquipmentTableRow(const UObject* WorldContextObj, FName EquipmentDataKey);

	static TSubclassOf<class APWVolumeActorBase> GetVolumeActorRandom(const UObject* WorldContextObj);

	const class UDataTable* GetDataTable(EDataTableType DataTableType) const;
	static const TSoftObjectPtr<class UNiagaraSystem> GetNiagara(const UObject* WorldContextObj, const struct FGameplayTag& GameplayTag);

protected:

	static UPWGameData* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EGameplayEffectType, TSubclassOf<class UGameplayEffect>> GameplayEffectMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<struct FGameplayTag, TSoftObjectPtr<class UNiagaraSystem>> NiagaraMap;

	// 내부에서 SoftObjectPtr AsyncLoad 하기 위해서 하드 레퍼런스로 가지기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable")
	class UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable")
	class UDataTable* EquipmentDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTable")
	class UDataTable* LevelDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class APWVolumeActorBase>> VolumeActorList;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class APWPlayerCharacter> PlayerCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class APWBuildableWall> BuildableWallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UMaterialInstance> SpawnPreviewMI;
};
