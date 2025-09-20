/*
 * Copyright (c) 2024-2025 PixelIndieDev
 *
 * This software is licensed under the MIT License.
 * You may use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, subject to the conditions of
 * the MIT License.
 *
 * See the LICENSE file in the root of this repository for full details.
 */

using UnrealBuildTool;

public class VertexLightingPlugin : ModuleRules
{
	public VertexLightingPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "InputCore",
                "RenderCore",
                "RHI",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
