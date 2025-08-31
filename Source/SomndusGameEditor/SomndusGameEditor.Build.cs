// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

using UnrealBuildTool;

public class SomndusGameEditor : ModuleRules
{
    public SomndusGameEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "SomndusGame"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Kismet",
                "BlueprintGraph",
                "KismetCompiler", 
                "UnrealEd",
                "SomndusGame"
            }
        );
    }
}