#pragma once

#include "CoreMinimal.h"
#include "SimpleTimeStructure.generated.h"

USTRUCT(BlueprintType)
struct FSimpleTimeStructure
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Time", meta = (ClampMin = "0", ClampMax = "23", UIMin = "0", UIMax = "23"))
    uint8 Hours;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simple Time", meta = (ClampMin = "0", ClampMax = "59", UIMin = "0", UIMax = "59"))
    uint8 Minutes;
};