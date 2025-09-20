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
#include "VertexLightingLight.h"
#include "Components/TimelineComponent.h"
#include "VertexLightingLightAnimated.generated.h"

/**
 * 
 */
UCLASS()
class VERTEXLIGHTINGPLUGIN_API AVertexLightingLightAnimated : public AVertexLightingLight
{
	GENERATED_BODY()

public:
	virtual void EnableActorTicking(bool Enable) override;

protected:
	virtual FVertexLightInfo GetSingleVertexLightInfo() override;

private:
	UFUNCTION(BlueprintCallable, Category = "Animated Light")
	bool SetupTimeline();

	UFUNCTION(BlueprintCallable, Category = "Animated Light")
	void PlayTimeline();

	UFUNCTION(BlueprintCallable, Category = "Animated Light")
	void StopTimeline();

	UFUNCTION()
	void OnTimelineUpdate(FLinearColor Value);

	UFUNCTION()
	void OnTimelineFinished();

	void AdvanceTimeline();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timers")
	FTimerHandle TimelineTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timers")
	FTimerHandle TimelineNoLoopTimerHandle;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ToolTip = "Defines the color animation curve."))
	UCurveLinearColor* ColorCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", Units = "s", ToolTip = "Starting point of the color curve."))
	float StartPosition = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ToolTip = "If true, the color used in the preview will use the 'LightColor' color.\n If false, it will use the starting 'StartPosition' on the 'ColorCurve' color even if it not a visible light color."))
	bool UseLightColorForPreview = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ToolTip = "Starts the curve at a random timestamp."))
	bool StartAtRandomPosition = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ToolTip = "Determines if the animation repeats. \nIf False, the light is destroyed after playback."))
	bool IsLooping = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ClampMin = "0.0", UIMin = "0.0", Units = "%", ToolTip = "Speed of the animation playback."))
	float PlayRate = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ClampMin = "0.0", UIMin = "0.0", Units = "s", ToolTip = "Time interval between Curve Light updates.\nSet to `0.0` to disable updates."))
	float CurveTickRate = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings | Animated Light", meta = (ExposeOnSpawn = "true", ToolTip = "Forces updates for animated lights even when tick rate updates are disabled."))
	bool ShouldUpdateNoTickRate = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultSettings | Animated Light | Icon", meta = (ExposeOnSpawn = "true", Units = "s", ToolTip = "This value gets used for the 2 colors that get used in the icon coloring."))
	float IconColorTimeValue01 = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultSettings | Animated Light | Icon", meta = (ExposeOnSpawn = "true", Units = "s", ToolTip = "This value gets used for the 2 colors that get used in the icon coloring."))
	float IconColorTimeValue02 = 1.5f;

	FTimeline Timeline;
};