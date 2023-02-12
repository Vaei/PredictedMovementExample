// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PredictedExample : ModuleRules
{
	public PredictedExample(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
