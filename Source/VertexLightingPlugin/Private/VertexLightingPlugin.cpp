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

#include "VertexLightingPlugin.h"
#include "VertexLightingPluginSettings.h"
#if WITH_EDITOR
    #include "ISettingsModule.h"
    #include "ISettingsSection.h"
#endif
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FVertexLightingPluginModule"

void FVertexLightingPluginModule::StartupModule()
{
    #if WITH_EDITOR
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->RegisterSettings("Project", "Plugins", "VertexLightingPlugin", FText::FromString("Vertex Lighting Plugin Settings"), FText::FromString("Configure Vertex Lighting Plugin settings"), GetMutableDefault<UVertexLightingPluginSettings>());
    }
    #endif
}

void FVertexLightingPluginModule::ShutdownModule()
{
    #if WITH_EDITOR
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "My Plugin");
    }
    #endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVertexLightingPluginModule, VertexLightingPlugin)