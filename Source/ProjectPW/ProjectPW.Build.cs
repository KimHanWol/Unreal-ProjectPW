﻿// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectPW : ModuleRules
{
	public ProjectPW(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"UMG",
            "Niagara",
            "GameplayAbilities",
            "GameplayTags",
            "OnlineSubsystem",
            "OnlineSubsystemSteam",
            "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        PublicIncludePaths.AddRange(new string[] { "ProjectPW" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
