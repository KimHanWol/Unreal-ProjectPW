// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "ProjectPW/Actor/Character/PWPlayerController.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePWPlayerController() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_APawn_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_APlayerController();
PROJECTPW_API UClass* Z_Construct_UClass_APWPlayerController();
PROJECTPW_API UClass* Z_Construct_UClass_APWPlayerController_NoRegister();
PROJECTPW_API UClass* Z_Construct_UClass_UMasterWidget_NoRegister();
PROJECTPW_API UClass* Z_Construct_UClass_UPWPlayerInputComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_ProjectPW();
// End Cross Module References

// Begin Class APWPlayerController Function CS_NextTurn
static FName NAME_APWPlayerController_CS_NextTurn = FName(TEXT("CS_NextTurn"));
void APWPlayerController::CS_NextTurn()
{
	ProcessEvent(FindFunctionChecked(NAME_APWPlayerController_CS_NextTurn),NULL);
}
struct Z_Construct_UFunction_APWPlayerController_CS_NextTurn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_APWPlayerController_CS_NextTurn_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_APWPlayerController, nullptr, "CS_NextTurn", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00220CC0, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_CS_NextTurn_Statics::Function_MetaDataParams), Z_Construct_UFunction_APWPlayerController_CS_NextTurn_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_APWPlayerController_CS_NextTurn()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_APWPlayerController_CS_NextTurn_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(APWPlayerController::execCS_NextTurn)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CS_NextTurn_Implementation();
	P_NATIVE_END;
}
// End Class APWPlayerController Function CS_NextTurn

// Begin Class APWPlayerController Function CS_Possess
struct PWPlayerController_eventCS_Possess_Parms
{
	APawn* PossessablePawn;
};
static FName NAME_APWPlayerController_CS_Possess = FName(TEXT("CS_Possess"));
void APWPlayerController::CS_Possess(APawn* PossessablePawn)
{
	PWPlayerController_eventCS_Possess_Parms Parms;
	Parms.PossessablePawn=PossessablePawn;
	ProcessEvent(FindFunctionChecked(NAME_APWPlayerController_CS_Possess),&Parms);
}
struct Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PossessablePawn;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::NewProp_PossessablePawn = { "PossessablePawn", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(PWPlayerController_eventCS_Possess_Parms, PossessablePawn), Z_Construct_UClass_APawn_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::NewProp_PossessablePawn,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_APWPlayerController, nullptr, "CS_Possess", nullptr, nullptr, Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::PropPointers), sizeof(PWPlayerController_eventCS_Possess_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00240CC1, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::Function_MetaDataParams), Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::Function_MetaDataParams) };
static_assert(sizeof(PWPlayerController_eventCS_Possess_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_APWPlayerController_CS_Possess()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_APWPlayerController_CS_Possess_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(APWPlayerController::execCS_Possess)
{
	P_GET_OBJECT(APawn,Z_Param_PossessablePawn);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->CS_Possess_Implementation(Z_Param_PossessablePawn);
	P_NATIVE_END;
}
// End Class APWPlayerController Function CS_Possess

// Begin Class APWPlayerController Function SC_ChangeInputEnabled
struct PWPlayerController_eventSC_ChangeInputEnabled_Parms
{
	bool bEnableCommander;
	bool bEnableCharacter;
};
static FName NAME_APWPlayerController_SC_ChangeInputEnabled = FName(TEXT("SC_ChangeInputEnabled"));
void APWPlayerController::SC_ChangeInputEnabled(bool bEnableCommander, bool bEnableCharacter)
{
	PWPlayerController_eventSC_ChangeInputEnabled_Parms Parms;
	Parms.bEnableCommander=bEnableCommander ? true : false;
	Parms.bEnableCharacter=bEnableCharacter ? true : false;
	ProcessEvent(FindFunctionChecked(NAME_APWPlayerController_SC_ChangeInputEnabled),&Parms);
}
struct Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bEnableCommander_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableCommander;
	static void NewProp_bEnableCharacter_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bEnableCharacter;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCommander_SetBit(void* Obj)
{
	((PWPlayerController_eventSC_ChangeInputEnabled_Parms*)Obj)->bEnableCommander = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCommander = { "bEnableCommander", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(PWPlayerController_eventSC_ChangeInputEnabled_Parms), &Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCommander_SetBit, METADATA_PARAMS(0, nullptr) };
void Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCharacter_SetBit(void* Obj)
{
	((PWPlayerController_eventSC_ChangeInputEnabled_Parms*)Obj)->bEnableCharacter = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCharacter = { "bEnableCharacter", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(PWPlayerController_eventSC_ChangeInputEnabled_Parms), &Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCharacter_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCommander,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::NewProp_bEnableCharacter,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_APWPlayerController, nullptr, "SC_ChangeInputEnabled", nullptr, nullptr, Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::PropPointers), sizeof(PWPlayerController_eventSC_ChangeInputEnabled_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x01040CC1, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::Function_MetaDataParams), Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::Function_MetaDataParams) };
static_assert(sizeof(PWPlayerController_eventSC_ChangeInputEnabled_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(APWPlayerController::execSC_ChangeInputEnabled)
{
	P_GET_UBOOL(Z_Param_bEnableCommander);
	P_GET_UBOOL(Z_Param_bEnableCharacter);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SC_ChangeInputEnabled_Implementation(Z_Param_bEnableCommander,Z_Param_bEnableCharacter);
	P_NATIVE_END;
}
// End Class APWPlayerController Function SC_ChangeInputEnabled

// Begin Class APWPlayerController Function SC_ChangeTurn
struct PWPlayerController_eventSC_ChangeTurn_Parms
{
	bool bMyTurn;
};
static FName NAME_APWPlayerController_SC_ChangeTurn = FName(TEXT("SC_ChangeTurn"));
void APWPlayerController::SC_ChangeTurn(bool bMyTurn)
{
	PWPlayerController_eventSC_ChangeTurn_Parms Parms;
	Parms.bMyTurn=bMyTurn ? true : false;
	ProcessEvent(FindFunctionChecked(NAME_APWPlayerController_SC_ChangeTurn),&Parms);
}
struct Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bMyTurn_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bMyTurn;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::NewProp_bMyTurn_SetBit(void* Obj)
{
	((PWPlayerController_eventSC_ChangeTurn_Parms*)Obj)->bMyTurn = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::NewProp_bMyTurn = { "bMyTurn", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(PWPlayerController_eventSC_ChangeTurn_Parms), &Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::NewProp_bMyTurn_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::NewProp_bMyTurn,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_APWPlayerController, nullptr, "SC_ChangeTurn", nullptr, nullptr, Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::PropPointers), sizeof(PWPlayerController_eventSC_ChangeTurn_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x01020CC0, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::Function_MetaDataParams), Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::Function_MetaDataParams) };
static_assert(sizeof(PWPlayerController_eventSC_ChangeTurn_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(APWPlayerController::execSC_ChangeTurn)
{
	P_GET_UBOOL(Z_Param_bMyTurn);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SC_ChangeTurn_Implementation(Z_Param_bMyTurn);
	P_NATIVE_END;
}
// End Class APWPlayerController Function SC_ChangeTurn

// Begin Class APWPlayerController Function SC_GameOver
struct PWPlayerController_eventSC_GameOver_Parms
{
	bool bWon;
};
static FName NAME_APWPlayerController_SC_GameOver = FName(TEXT("SC_GameOver"));
void APWPlayerController::SC_GameOver(bool bWon)
{
	PWPlayerController_eventSC_GameOver_Parms Parms;
	Parms.bWon=bWon ? true : false;
	ProcessEvent(FindFunctionChecked(NAME_APWPlayerController_SC_GameOver),&Parms);
}
struct Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static void NewProp_bWon_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bWon;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
void Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::NewProp_bWon_SetBit(void* Obj)
{
	((PWPlayerController_eventSC_GameOver_Parms*)Obj)->bWon = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::NewProp_bWon = { "bWon", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(PWPlayerController_eventSC_GameOver_Parms), &Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::NewProp_bWon_SetBit, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::NewProp_bWon,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_APWPlayerController, nullptr, "SC_GameOver", nullptr, nullptr, Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::PropPointers), sizeof(PWPlayerController_eventSC_GameOver_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x01020CC0, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::Function_MetaDataParams), Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::Function_MetaDataParams) };
static_assert(sizeof(PWPlayerController_eventSC_GameOver_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_APWPlayerController_SC_GameOver()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_APWPlayerController_SC_GameOver_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(APWPlayerController::execSC_GameOver)
{
	P_GET_UBOOL(Z_Param_bWon);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SC_GameOver_Implementation(Z_Param_bWon);
	P_NATIVE_END;
}
// End Class APWPlayerController Function SC_GameOver

// Begin Class APWPlayerController Function SM_OnHealthChanged
struct PWPlayerController_eventSM_OnHealthChanged_Parms
{
	AActor* TargetActor;
	float MaxHealth;
	float CurrentHealth;
};
static FName NAME_APWPlayerController_SM_OnHealthChanged = FName(TEXT("SM_OnHealthChanged"));
void APWPlayerController::SM_OnHealthChanged(AActor* TargetActor, float MaxHealth, float CurrentHealth)
{
	PWPlayerController_eventSM_OnHealthChanged_Parms Parms;
	Parms.TargetActor=TargetActor;
	Parms.MaxHealth=MaxHealth;
	Parms.CurrentHealth=CurrentHealth;
	ProcessEvent(FindFunctionChecked(NAME_APWPlayerController_SM_OnHealthChanged),&Parms);
}
struct Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_TargetActor;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_MaxHealth;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_CurrentHealth;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::NewProp_TargetActor = { "TargetActor", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(PWPlayerController_eventSM_OnHealthChanged_Parms, TargetActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::NewProp_MaxHealth = { "MaxHealth", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(PWPlayerController_eventSM_OnHealthChanged_Parms, MaxHealth), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::NewProp_CurrentHealth = { "CurrentHealth", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(PWPlayerController_eventSM_OnHealthChanged_Parms, CurrentHealth), METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::NewProp_TargetActor,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::NewProp_MaxHealth,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::NewProp_CurrentHealth,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_APWPlayerController, nullptr, "SM_OnHealthChanged", nullptr, nullptr, Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::PropPointers), sizeof(PWPlayerController_eventSM_OnHealthChanged_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00024CC0, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::Function_MetaDataParams), Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::Function_MetaDataParams) };
static_assert(sizeof(PWPlayerController_eventSM_OnHealthChanged_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(APWPlayerController::execSM_OnHealthChanged)
{
	P_GET_OBJECT(AActor,Z_Param_TargetActor);
	P_GET_PROPERTY(FFloatProperty,Z_Param_MaxHealth);
	P_GET_PROPERTY(FFloatProperty,Z_Param_CurrentHealth);
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->SM_OnHealthChanged_Implementation(Z_Param_TargetActor,Z_Param_MaxHealth,Z_Param_CurrentHealth);
	P_NATIVE_END;
}
// End Class APWPlayerController Function SM_OnHealthChanged

// Begin Class APWPlayerController
void APWPlayerController::StaticRegisterNativesAPWPlayerController()
{
	UClass* Class = APWPlayerController::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CS_NextTurn", &APWPlayerController::execCS_NextTurn },
		{ "CS_Possess", &APWPlayerController::execCS_Possess },
		{ "SC_ChangeInputEnabled", &APWPlayerController::execSC_ChangeInputEnabled },
		{ "SC_ChangeTurn", &APWPlayerController::execSC_ChangeTurn },
		{ "SC_GameOver", &APWPlayerController::execSC_GameOver },
		{ "SM_OnHealthChanged", &APWPlayerController::execSM_OnHealthChanged },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APWPlayerController);
UClass* Z_Construct_UClass_APWPlayerController_NoRegister()
{
	return APWPlayerController::StaticClass();
}
struct Z_Construct_UClass_APWPlayerController_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Collision Rendering Transformation" },
		{ "IncludePath", "Actor/Character/PWPlayerController.h" },
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MasterWidget_MetaData[] = {
		{ "Category", "PWPlayerController" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlayerInputComponent_MetaData[] = {
		{ "Category", "PWPlayerController" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "//Component\n" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Component" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIsMyTurn_MetaData[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bTurnDataDirty_MetaData[] = {
		{ "ModuleRelativePath", "Actor/Character/PWPlayerController.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MasterWidget;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_PlayerInputComponent;
	static void NewProp_bIsMyTurn_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsMyTurn;
	static void NewProp_bTurnDataDirty_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bTurnDataDirty;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_APWPlayerController_CS_NextTurn, "CS_NextTurn" }, // 3012562232
		{ &Z_Construct_UFunction_APWPlayerController_CS_Possess, "CS_Possess" }, // 2247700066
		{ &Z_Construct_UFunction_APWPlayerController_SC_ChangeInputEnabled, "SC_ChangeInputEnabled" }, // 4274173615
		{ &Z_Construct_UFunction_APWPlayerController_SC_ChangeTurn, "SC_ChangeTurn" }, // 67996181
		{ &Z_Construct_UFunction_APWPlayerController_SC_GameOver, "SC_GameOver" }, // 4236419563
		{ &Z_Construct_UFunction_APWPlayerController_SM_OnHealthChanged, "SM_OnHealthChanged" }, // 1776219217
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APWPlayerController>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_APWPlayerController_Statics::NewProp_MasterWidget = { "MasterWidget", nullptr, (EPropertyFlags)0x0040000000090009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(APWPlayerController, MasterWidget), Z_Construct_UClass_UMasterWidget_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MasterWidget_MetaData), NewProp_MasterWidget_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_APWPlayerController_Statics::NewProp_PlayerInputComponent = { "PlayerInputComponent", nullptr, (EPropertyFlags)0x0040000000080009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(APWPlayerController, PlayerInputComponent), Z_Construct_UClass_UPWPlayerInputComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlayerInputComponent_MetaData), NewProp_PlayerInputComponent_MetaData) };
void Z_Construct_UClass_APWPlayerController_Statics::NewProp_bIsMyTurn_SetBit(void* Obj)
{
	((APWPlayerController*)Obj)->bIsMyTurn = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_APWPlayerController_Statics::NewProp_bIsMyTurn = { "bIsMyTurn", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(APWPlayerController), &Z_Construct_UClass_APWPlayerController_Statics::NewProp_bIsMyTurn_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIsMyTurn_MetaData), NewProp_bIsMyTurn_MetaData) };
void Z_Construct_UClass_APWPlayerController_Statics::NewProp_bTurnDataDirty_SetBit(void* Obj)
{
	((APWPlayerController*)Obj)->bTurnDataDirty = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_APWPlayerController_Statics::NewProp_bTurnDataDirty = { "bTurnDataDirty", nullptr, (EPropertyFlags)0x0040000000002000, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(APWPlayerController), &Z_Construct_UClass_APWPlayerController_Statics::NewProp_bTurnDataDirty_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bTurnDataDirty_MetaData), NewProp_bTurnDataDirty_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_APWPlayerController_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APWPlayerController_Statics::NewProp_MasterWidget,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APWPlayerController_Statics::NewProp_PlayerInputComponent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APWPlayerController_Statics::NewProp_bIsMyTurn,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_APWPlayerController_Statics::NewProp_bTurnDataDirty,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_APWPlayerController_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_APWPlayerController_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APlayerController,
	(UObject* (*)())Z_Construct_UPackage__Script_ProjectPW,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_APWPlayerController_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_APWPlayerController_Statics::ClassParams = {
	&APWPlayerController::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	Z_Construct_UClass_APWPlayerController_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	UE_ARRAY_COUNT(Z_Construct_UClass_APWPlayerController_Statics::PropPointers),
	0,
	0x009002A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_APWPlayerController_Statics::Class_MetaDataParams), Z_Construct_UClass_APWPlayerController_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_APWPlayerController()
{
	if (!Z_Registration_Info_UClass_APWPlayerController.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APWPlayerController.OuterSingleton, Z_Construct_UClass_APWPlayerController_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_APWPlayerController.OuterSingleton;
}
template<> PROJECTPW_API UClass* StaticClass<APWPlayerController>()
{
	return APWPlayerController::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(APWPlayerController);
APWPlayerController::~APWPlayerController() {}
// End Class APWPlayerController

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_hanwol_kim_Desktop_ProjectPW_Source_ProjectPW_Actor_Character_PWPlayerController_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_APWPlayerController, APWPlayerController::StaticClass, TEXT("APWPlayerController"), &Z_Registration_Info_UClass_APWPlayerController, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APWPlayerController), 3720727456U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_hanwol_kim_Desktop_ProjectPW_Source_ProjectPW_Actor_Character_PWPlayerController_h_3181121503(TEXT("/Script/ProjectPW"),
	Z_CompiledInDeferFile_FID_Users_hanwol_kim_Desktop_ProjectPW_Source_ProjectPW_Actor_Character_PWPlayerController_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_hanwol_kim_Desktop_ProjectPW_Source_ProjectPW_Actor_Character_PWPlayerController_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
