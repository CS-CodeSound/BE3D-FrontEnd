// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class BE3D : ModuleRules
{
    public BE3D(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", "CoreUObject", "Engine", "InputCore",
            "Json", "JsonUtilities",
            "AssetRegistry",
            "HTTP"
        });

        // 에디터 전용 모듈은 에디터 빌드에만 포함
        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "AssetTools", "UnrealEd" });
        }

        PrivateDependencyModuleNames.AddRange(new string[] { });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

