// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SomndusGame : ModuleRules
{
	public SomndusGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "UMG", 
				"InputCore", "EnhancedInput",
				"StructUtils",
				"CommonUI", "CommonInput", "CommonUser", "CommonGame",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine", "NetCore",
				"Slate",
				"SlateCore",
				"GameplayTags", "StructUtils",
				"InputCore", "EnhancedInput",
				"CommonUI", "CommonInput", "CommonGame", "CommonUser"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
