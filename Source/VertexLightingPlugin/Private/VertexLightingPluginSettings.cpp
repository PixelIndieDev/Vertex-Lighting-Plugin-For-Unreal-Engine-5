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

#include "VertexLightingPluginSettings.h"
#include "VertexLightingFunctionLibrary_C.h"

#if WITH_EDITOR
void UVertexLightingPluginSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    MaxLights = FMath::RoundUpToPowerOfTwo(FMath::Max(1, MaxLights));

    if (VertexLightRenderTarget.IsValid())
    {
        UTextureRenderTarget2D* RT = VertexLightRenderTarget.Get();
        if (RT)
        {
            int32 TargetSize = UVertexLightingFunctionLibrary_C::GetVertexLightTextureDataSize(MaxLights);

            RT->Modify();
            RT->InitCustomFormat(TargetSize, TargetSize, PF_FloatRGBA, false);
            RT->UpdateResourceImmediate(true);
        }
    }
}
#endif
