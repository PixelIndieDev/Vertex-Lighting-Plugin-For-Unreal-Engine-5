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

#include "VertexLightingFunctionLibrary_C.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RenderTargetPool.h"
#include "RHICommandList.h"
#include "RenderUtils.h"
#include "TextureResource.h"

void UVertexLightingFunctionLibrary_C::DrawVertexLightInformationToRenderTarget(UTextureRenderTarget2D* RenderTarget, const TArray<FLinearColor>& VertexLights_Locations, const TArray<FLinearColor>& VertexLights_Colors)
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("RenderTarget is not valid"));
        return;
    }

    int32 Width = RenderTarget->SizeX;
    int32 Height = RenderTarget->SizeY;

    int32 NumLights = FMath::Min(VertexLights_Locations.Num(), VertexLights_Colors.Num());

    // Each light uses 2 rows. Check capacity.
    if (IsRenderTargetRightSize(RenderTarget, NumLights, Width, Height))
    {
        UE_LOG(LogTemp, Warning, TEXT("Too many lights (%d)."), NumLights);
    }

    // Prepare pixel buffer
    TArray<FFloat16Color> PixelData;
    PixelData.SetNumZeroed(Width * Height);

    for (int32 LightIndex = 0; LightIndex < NumLights; ++LightIndex)
    {
        int32 RowPair = LightIndex / Width;     // which 2-row pair this light belongs in
        int32 Col = LightIndex % Width;         // column in that row
        int32 RowLoc = RowPair * 2;             // row for location
        int32 RowColor = RowLoc + 1;            // row for color

        PixelData[RowLoc * Width + Col] = FFloat16Color(VertexLights_Locations[LightIndex]);
        PixelData[RowColor * Width + Col] = FFloat16Color(VertexLights_Colors[LightIndex]);
    }

    FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();

    ENQUEUE_RENDER_COMMAND(UpdateRenderTargetCommand)(
        [RTResource, PixelDataCopy = MoveTemp(PixelData), Width, Height](FRHICommandListImmediate& RHICmdList)
        {
            FRHITexture* Texture = RTResource->GetRenderTargetTexture();
            if (!Texture)
                return;

            uint32 SrcPitch = Width * sizeof(FFloat16Color);
            RHICmdList.UpdateTexture2D(Texture, 0, FUpdateTextureRegion2D(0, 0, 0, 0, Width, Height), SrcPitch, reinterpret_cast<const uint8*>(PixelDataCopy.GetData()));
        }
    );
}

void UVertexLightingFunctionLibrary_C::VertexLight_Register(FVertexLightInfo VertexLightInfo, AVertexLightingLight* RegisterActorReference, AVertexLightingManagerActor* ManagerReference)
{
    if (IsValid(ManagerReference)) {
        if (RegisterActorReference && VertexLightInfo.Radius) {
            RegisterLight(VertexLightInfo, RegisterActorReference, ManagerReference);
            ManagerReference->ShouldManagerUpdateLights();
        }
    }
    else {
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(801, 20.0f, FColor::Red, "VertextLighting ERROR\n--------------------\nNo VertexLighting_Manager found");
        #endif
    }
}

void UVertexLightingFunctionLibrary_C::VertexLight_Unregister(AVertexLightingLight* RegisterActorReference, AVertexLightingManagerActor* ManagerReference)
{
    if (ManagerReference) {
        if (ManagerReference->VertexLightArray.Contains(RegisterActorReference)) {
            ManagerReference->VertexLightArray.Remove(RegisterActorReference);
            ManagerReference->ShouldManagerUpdateLights();
        }
    }
    else {
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(801, 20.0f, FColor::Red, "VertextLighting ERROR\n--------------------\nNo VertexLighting_Manager found");
        #endif
    }
}

void UVertexLightingFunctionLibrary_C::VertexLightAnimated_RegisterUpdate(FVertexLightInfo VertexLightInfo, AVertexLightingLight* RegisterActorReference, AVertexLightingManagerActor* ManagerReference, bool ShouldUpdateNoTickRate)
{
    if (ManagerReference) {
        if (RegisterActorReference && VertexLightInfo.Radius) {
            RegisterLight(VertexLightInfo, RegisterActorReference, ManagerReference);

            if (ShouldUpdateNoTickRate) {
                ManagerReference->ShouldManagerUpdateLights();
            }
        }
    }
    else {
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(801, 20.0f, FColor::Red, "VertextLighting ERROR\n--------------------\nNo VertexLighting_Manager found");
        #endif
    }
}

FVector UVertexLightingFunctionLibrary_C::LocationToVertexLightLocation(FVector Location)
{
    return (Location/100);
}

float UVertexLightingFunctionLibrary_C::TimeToPercentage(FSimpleTimeStructure Time)
{
    return ((static_cast<float>((Time.Hours * 60) + Time.Minutes)) / 1440) * 100;
}

int32 UVertexLightingFunctionLibrary_C::GetVertexLightTextureDataSize(int32 VertexLightCount)
{
    int32 RequiredPixels = VertexLightCount * 2;

    //Find smallest square
    int32 SideLength = FMath::CeilToInt(FMath::Sqrt((float)RequiredPixels));

    // Make sure it's a power of two
    int32 PowOf2SideLength = FMath::RoundUpToPowerOfTwo(SideLength);

    return PowOf2SideLength;
}

bool UVertexLightingFunctionLibrary_C::IsRenderTargetRightSize(UTextureRenderTarget2D* RenderTarget, int32 MaxLights)
{
    int32 Width = RenderTarget->SizeX;
    int32 Height = RenderTarget->SizeY;

    // Each light uses 2 rows. Check capacity.
    int32 MaxLightsOnTarget = (Height / 2) * Width;
    if (MaxLights > MaxLightsOnTarget) {
        return true;
    }
    else {
        return false;
    }
}

bool UVertexLightingFunctionLibrary_C::IsRenderTargetRightSize(UTextureRenderTarget2D* RenderTarget, int32 MaxLights, int32 Width, int32 Height)
{
    int32 MaxLightsOnTarget = (Height / 2) * Width;
    if (MaxLights > MaxLightsOnTarget) {
        return true;
    }
    else {
        return false;
    }
}

void UVertexLightingFunctionLibrary_C::RegisterLight(FVertexLightInfo& VertexLightInfo, AVertexLightingLight*& RegisterActorReference, AVertexLightingManagerActor*& ManagerReference) {
    
    FVertexLightingStructure passthruStructure;

    FVector convertedLocation = UVertexLightingFunctionLibrary_C::LocationToVertexLightLocation(VertexLightInfo.Location);
    passthruStructure.LocationAndRadius = FVector4(convertedLocation.X, convertedLocation.Y, convertedLocation.Z, VertexLightInfo.Radius);
    FLinearColor convertedColor = VertexLightInfo.LightColor * VertexLightInfo.Brightness;
    passthruStructure.ColorAndInfluenceOnNormal = FLinearColor(convertedColor.R, convertedColor.G, convertedColor.B, VertexLightInfo.InfluenceToNormal);
    passthruStructure.IsMoveableLight = VertexLightInfo.bIsMoveable;

    ManagerReference->VertexLightArray.Add(RegisterActorReference, passthruStructure);
}