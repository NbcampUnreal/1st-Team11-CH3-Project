// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTPS_Project_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_TPS_Project;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_TPS_Project()
	{
		if (!Z_Registration_Info_UPackage__Script_TPS_Project.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/TPS_Project",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0xE017D3EA,
				0x0200C8DA,
				METADATA_PARAMS(0, nullptr)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_TPS_Project.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_TPS_Project.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_TPS_Project(Z_Construct_UPackage__Script_TPS_Project, TEXT("/Script/TPS_Project"), Z_Registration_Info_UPackage__Script_TPS_Project, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0xE017D3EA, 0x0200C8DA));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
