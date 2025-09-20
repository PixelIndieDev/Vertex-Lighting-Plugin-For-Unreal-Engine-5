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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "VertexLightInfo.h"
#include "VertexLightingManagerActor.h"
#include "RHI.h"
#include "RenderResource.h"
#include "RHIResources.h"
#include "RHIUtilities.h"
#include "RenderGraphUtils.h"
#include "RHICommandList.h"
#include "GlobalShader.h"
#include "ShaderParameterUtils.h"
#include "CommonRenderResources.h"
#include "VertexLightingFunctionLibrary_C.generated.h"

struct FVertexLightData
{
	FVector4 LocationRadius;
	FVector4 ColorAlpha;
};

/**
 * 
 */
UCLASS()
class VERTEXLIGHTINGPLUGIN_API UVertexLightingFunctionLibrary_C : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Vertex Lighting")
    static void DrawVertexLightInformationToRenderTarget(UTextureRenderTarget2D* RenderTarget, const TArray<FLinearColor>& VertexLights_Locations, const TArray<FLinearColor>& VertexLights_Colors);

	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting")
	static void VertexLight_Register(FVertexLightInfo VertexLightInfo, AVertexLightingLight* RegisterActorReference, AVertexLightingManagerActor* ManagerReference);

	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting")
	static void VertexLight_Unregister(AVertexLightingLight* RegisterActorReference, AVertexLightingManagerActor* ManagerReference);

	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting Animated")
	static void VertexLightAnimated_RegisterUpdate(FVertexLightInfo VertexLightInfo, AVertexLightingLight* RegisterActorReference, AVertexLightingManagerActor* ManagerReference, bool ShouldUpdateNoTickRate);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Lighting")
	static FVector LocationToVertexLightLocation(FVector Location);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Lighting | Time")
	static float TimeToPercentage(FSimpleTimeStructure Time);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Lighting | TextureRenderTarget2D")
	static int32 GetVertexLightTextureDataSize(int32 VertexLightCount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Lighting | TextureRenderTarget2D")
	static bool IsRenderTargetRightSize(UTextureRenderTarget2D* RenderTarget, int32 MaxLights);

	static bool IsRenderTargetRightSize(UTextureRenderTarget2D* RenderTarget, int32 MaxLights, int32 Width, int32 Height);

private:
	UFUNCTION()
	static void RegisterLight(FVertexLightInfo& VertexLightInfo, AVertexLightingLight*& RegisterActorReference, AVertexLightingManagerActor*& ManagerReference);
};
