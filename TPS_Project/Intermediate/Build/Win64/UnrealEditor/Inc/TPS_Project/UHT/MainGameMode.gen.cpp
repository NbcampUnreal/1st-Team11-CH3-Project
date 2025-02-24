// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TPS_Project/Public/MainGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMainGameMode() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_AGameMode();
TPS_PROJECT_API UClass* Z_Construct_UClass_AMainGameMode();
TPS_PROJECT_API UClass* Z_Construct_UClass_AMainGameMode_NoRegister();
UPackage* Z_Construct_UPackage__Script_TPS_Project();
// End Cross Module References

// Begin Class AMainGameMode
void AMainGameMode::StaticRegisterNativesAMainGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMainGameMode);
UClass* Z_Construct_UClass_AMainGameMode_NoRegister()
{
	return AMainGameMode::StaticClass();
}
struct Z_Construct_UClass_AMainGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "MainGameMode.h" },
		{ "ModuleRelativePath", "Public/MainGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMainGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AMainGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameMode,
	(UObject* (*)())Z_Construct_UPackage__Script_TPS_Project,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMainGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AMainGameMode_Statics::ClassParams = {
	&AMainGameMode::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x009002ACu,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMainGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_AMainGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AMainGameMode()
{
	if (!Z_Registration_Info_UClass_AMainGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMainGameMode.OuterSingleton, Z_Construct_UClass_AMainGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AMainGameMode.OuterSingleton;
}
template<> TPS_PROJECT_API UClass* StaticClass<AMainGameMode>()
{
	return AMainGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AMainGameMode);
AMainGameMode::~AMainGameMode() {}
// End Class AMainGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_git_work_space_TPS_Project_1st_Team11_CH3_Project_TPS_Project_Source_TPS_Project_Public_MainGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AMainGameMode, AMainGameMode::StaticClass, TEXT("AMainGameMode"), &Z_Registration_Info_UClass_AMainGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMainGameMode), 1987682458U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_git_work_space_TPS_Project_1st_Team11_CH3_Project_TPS_Project_Source_TPS_Project_Public_MainGameMode_h_2154030055(TEXT("/Script/TPS_Project"),
	Z_CompiledInDeferFile_FID_git_work_space_TPS_Project_1st_Team11_CH3_Project_TPS_Project_Source_TPS_Project_Public_MainGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_git_work_space_TPS_Project_1st_Team11_CH3_Project_TPS_Project_Source_TPS_Project_Public_MainGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
