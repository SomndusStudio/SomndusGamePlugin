// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

using UnrealBuildTool;

public class SomndusGame : ModuleRules
{
	public SomndusGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
		PrivatePCHHeaderFile = "Public/SomndusGame.h";

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core", "UMG",
				"InputCore", "EnhancedInput", "DeveloperSettings",
				"StructUtils",
				"CommonUI", "CommonInput", "CommonUser", "CommonGame", "GameSettings",
				"MovieScene", "LevelSequence",
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
				"CommonUI", "CommonInput", "CommonGame", "CommonUser", "AudioModulation"
			}
		);
	}
}