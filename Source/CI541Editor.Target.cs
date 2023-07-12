// Copyright (c) 2020-2022 Jack C. Lloyd. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CI541EditorTarget : TargetRules
{
	public CI541EditorTarget(TargetInfo Target) : base(Target)
	{
		bOverrideBuildEnvironment = true;
		//AdditionalCompilerArguments = "-Wno-unused-but-set-variable";

		DefaultBuildSettings = BuildSettingsVersion.V2;
		bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Warning;

		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "CI541" } );
	}
}
