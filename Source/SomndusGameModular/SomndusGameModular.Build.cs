using UnrealBuildTool;

public class SomndusGameModular : ModuleRules
{
    public SomndusGameModular(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		
        PrivatePCHHeaderFile = "Public/SomndusGameModular.h";
        
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                // Core Modules
                "Core", "CoreUObject", "ApplicationCore", "Engine",
                              
                "GameplayTags",
                
                // Gameplay Framework
                "ModularGameplay", "ModularGameplayActors", "GameFeatures",
                
                // UI
                "UMG", "UIExtension", "CommonUI", "CommonInput", "CommonUser", "CommonGame",
                
                // Additional Frameworks
                "DataRegistry", "StructUtils", "PropertyPath",
                
                "SomndusGame", "CommonLoadingScreen",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "NetCore",
            }
        );
    }
}