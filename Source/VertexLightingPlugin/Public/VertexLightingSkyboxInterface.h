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
#include "UObject/Interface.h"
#include "SimpleTimeStructure.h"
#include "VertexLightingSkyboxInterface.generated.h"

UINTERFACE(MinimalAPI)
class UVertexLightingSkyboxInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VERTEXLIGHTINGPLUGIN_API IVertexLightingSkyboxInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Vertex Lighting Interface")
	void UpdateDayNightTime(FSimpleTimeStructure Time, UCurveLinearColor* DayNightCycleColorCurve_manager);
};
