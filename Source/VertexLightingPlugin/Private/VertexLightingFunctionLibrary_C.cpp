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

    int32 NumCol = FMath::Min(VertexLights_Locations.Num(), VertexLights_Colors.Num());

    if (Width < NumCol || Height < 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("RenderTarget must be at least Nx2 in size."));
        return;
    }

    // Prepare pixel buffer
    TArray<FFloat16Color> PixelData;
    PixelData.SetNumZeroed(Width * Height);

    for (int32 X = 0; X < NumCol; ++X)
    {
        PixelData[0 * Width + X] = FFloat16Color(VertexLights_Locations[X]); // First row
        PixelData[1 * Width + X] = FFloat16Color(VertexLights_Colors[X]); // Second row
    }

    FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource();

    ENQUEUE_RENDER_COMMAND(UpdateRenderTargetCommand)([RTResource, PixelDataCopy = MoveTemp(PixelData), Width, Height](FRHICommandListImmediate& RHICmdList)
        {
            FRHITexture* Texture = RTResource->GetRenderTargetTexture();
            if (!Texture)
                return;

            uint32 SrcPitch = Width * sizeof(FFloat16Color);
            void* TempMemory = FMemory::Malloc(SrcPitch * Height);
            FMemory::Memcpy(TempMemory, PixelDataCopy.GetData(), SrcPitch * Height);

            FUpdateTextureRegion2D Region(0, 0, 0, 0, Width, Height);
            RHICmdList.UpdateTexture2D(Texture, 0, Region, SrcPitch, static_cast<uint8*>(TempMemory));

            FMemory::Free(TempMemory);
        }
    );
}
