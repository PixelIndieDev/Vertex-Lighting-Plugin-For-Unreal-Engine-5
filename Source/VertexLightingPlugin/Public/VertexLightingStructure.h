#pragma once

#include "CoreMinimal.h"
#include "VertexLightingStructure.generated.h"

USTRUCT(BlueprintType)
struct FVertexLightingStructure
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FVector4 LocationAndRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color")
    FLinearColor ColorAndInfluenceOnNormal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moveable")
    bool IsMoveableLight;
};