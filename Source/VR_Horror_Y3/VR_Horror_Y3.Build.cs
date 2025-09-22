// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VR_Horror_Y3 : ModuleRules
{
	public VR_Horror_Y3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "VRExpansionPlugin", "NavigationSystem", "GameplayTags"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
