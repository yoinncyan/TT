// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MTest002 : ModuleRules
{
	public MTest002(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "AnimationCore", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "AnimGraphRuntime"});
    }
}
