// Copyright 2023 Fengzz. All Rights Reserved.




using UnrealBuildTool;

public class AnimationMagicEditor : ModuleRules
{
	public AnimationMagicEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = false;
        PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...

            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"AnimationMagicEditor",
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "AnimationMagic",
                "Slate",
                "SlateCore",
                "AnimGraphRuntime",
                "BlueprintGraph",
                "AnimGraph",
                "AnimationEditMode",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"GraphEditor",
                "ToolMenus",
                "KismetCompiler",
                "Kismet",
                "ToolWidgets",
                "AnimationEditMode",

                "CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "EditorStyle",
                "UnrealEd",
				"EditorFramework",
                "AnimationMagic",
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
