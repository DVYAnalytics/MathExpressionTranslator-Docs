// Copyright 2026 LostInThought. All Rights Reserved.

using UnrealBuildTool;

public class MathTranslator : ModuleRules
{
	public MathTranslator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);
	}
}