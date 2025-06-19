#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/TextureRenderTarget2D.h"
#include "VertexLightingFunctionLibrary_C.generated.h"

/**
 * 
 */
UCLASS()
class VERTEXLIGHTINGPLUGIN_API UVertexLightingFunctionLibrary_C : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "Vertex Lighting")
    static void DrawVertexLightInformationToRenderTarget(UTextureRenderTarget2D* RenderTarget,const TArray<FLinearColor>& VertexLights_Locations,const TArray<FLinearColor>& VertexLights_Colors);
};
