// LINK

#pragma once

//Engine
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

//Game

//Default
#include "PWGameData.generated.h"


UENUM()
enum EGameplayEffectType
{
	AddHealth,
};

UENUM()
enum EDataTableType
{
	Character,
};

UCLASS()
class PROJECTPW_API UPWGameData: public UDataAsset
{
	GENERATED_BODY()

public:

	static const UPWGameData* Get(const UObject* WorldContextObj);

	static TSubclassOf<class UGameplayEffect> GetGameplayEffect(const UObject* WorldContextObj, EGameplayEffectType GameplayEffectType);

	template <typename TRowType>
	const TRowType* FindTableRow(EDataTableType DataTableType, const FName& KeyName) const
	{
		for (TTuple<TEnumAsByte<EDataTableType>, TSoftObjectPtr<class UDataTable>>DataTableData : DataTableMap)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DataTableData.Value.Get()->GetName());
		}

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

	virtual void PostLoad() override;

protected:

	static UPWGameData* Instance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TEnumAsByte<EGameplayEffectType>, TSubclassOf<class UGameplayEffect>> GameplayEffectMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TEnumAsByte<EDataTableType>, TSoftObjectPtr<class UDataTable>> DataTableMap;

};
