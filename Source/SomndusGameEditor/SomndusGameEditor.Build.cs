// Copyright (C) Schartier Isaac - Official Documentation: https://www.somndus-studio.com

using UnrealBuildTool;

public class SomndusGameEditor : ModuleRules
{
    public SomndusGameEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", "SomndusGame", "EditorWidgets", "EditorStyle", "AssetTools"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "InputCore",
                "Engine",
                
                // UI / Slate
                "Slate",
                "SlateCore",
                "EditorStyle",
                "ToolMenus",
                "EditorWidgets",
                
                // Blueprint / Kismet
                "BlueprintGraph",
                "Kismet",
                "KismetCompiler",
                "KismetWidgets",
                
                
                // Editor Tools / Systems
                "UnrealEd",
                "AssetTools",
                "ClassViewer",
                "DeveloperSettings",
                "GameplayTags",
                "Projects",
                "PropertyEditor",
                "GraphEditor",
                
                "EnhancedInput",
            }
        );
    }
}