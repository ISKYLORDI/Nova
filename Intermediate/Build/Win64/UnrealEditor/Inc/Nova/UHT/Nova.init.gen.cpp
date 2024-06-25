// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNova_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_Nova;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_Nova()
	{
		if (!Z_Registration_Info_UPackage__Script_Nova.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/Nova",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000000,
				0x0D19ABDB,
				0x63E2966F,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_Nova.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_Nova.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_Nova(Z_Construct_UPackage__Script_Nova, TEXT("/Script/Nova"), Z_Registration_Info_UPackage__Script_Nova, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x0D19ABDB, 0x63E2966F));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
