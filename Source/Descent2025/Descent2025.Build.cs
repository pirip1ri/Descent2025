// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Descent2025 : ModuleRules
{
	public Descent2025(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
