#pragma once

#include "CoreMinimal.h"
#include "VertexLightInfo.generated.h"

USTRUCT(BlueprintType)
struct FVertexLightInfo
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertex Light")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertex Light")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertex Light")
	FLinearColor LightColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertex Light")
	float Brightness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertex Light")
	float InfluenceToNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertex Light")
	bool bIsMoveable;
};