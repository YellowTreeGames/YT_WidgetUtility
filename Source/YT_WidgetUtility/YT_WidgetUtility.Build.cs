// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class YT_WidgetUtility : ModuleRules
{
	public YT_WidgetUtility(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"UMG",
				"Slate",
				"SlateCore",
				"AudioWidgets",
				"GameplayTags",
				"CommonUI",
				"YT_Utility"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings",
				"YT_Helpers"
			}
		);
	}
}
