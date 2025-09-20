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

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/TextureRenderTarget2D.h"
#include "VertexLightingPluginSettings.generated.h"

UCLASS(config = VertexLightingPlugin, defaultconfig, meta = (DisplayName = "Vertex Lighting Plugin Settings"))
class VERTEXLIGHTINGPLUGIN_API UVertexLightingPluginSettings : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, config, Category = "Vertex Lights", meta = (ClampMin = "0", UIMin = "0", UIMax = "4096"))
    int32 MaxLights;

    UPROPERTY(EditAnywhere, config, Category = "Vertex Lights", meta = (AllowedClasses = "TextureRenderTarget2D", ToolTip = "The render target selected here will resize when you edit the 'MaxLights'."))
    TSoftObjectPtr<UTextureRenderTarget2D> VertexLightRenderTarget;

    #if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    #endif
};