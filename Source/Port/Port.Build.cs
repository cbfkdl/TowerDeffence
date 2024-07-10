// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Port : ModuleRules
{
	public Port(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "Port" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GamePlayTasks", "SlateCore"});
	}
}
