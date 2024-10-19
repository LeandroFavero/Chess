// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Chess : ModuleRules
{
	public Chess(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine", 
			"InputCore", 
			"RenderCore",
			"Sockets", /*atm only to find the local ip*/
			"ApplicationCore", /*CopyToClipboard*/
			"OnlineBase"
		});

		//PrivateDependencyModuleNames.AddRange(new string[] { });

		PrivateIncludePaths.AddRange(new string[] { "Chess" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });
		
		// Uncomment if you are using online features
		PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
