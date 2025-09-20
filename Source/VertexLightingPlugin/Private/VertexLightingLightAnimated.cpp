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

#include "VertexLightingLightAnimated.h"
#include "VertexLightingFunctionLibrary_C.h"
#include "Curves/CurveLinearColor.h"
#include "Engine/World.h"
#include "TimerManager.h"

void AVertexLightingLightAnimated::EnableActorTicking(bool Enable)
{
    PrimaryActorTick.bCanEverTick = Enable;

    if (Enable) {
        if (GetWorld()->GetTimerManager().IsTimerPaused(TimelineTimerHandle)) {
            GetWorld()->GetTimerManager().UnPauseTimer(TimelineTimerHandle);
        }

        if (GetWorld()->GetTimerManager().IsTimerPaused(TimelineNoLoopTimerHandle)) {
            GetWorld()->GetTimerManager().UnPauseTimer(TimelineNoLoopTimerHandle);
        }
    }
    else {
        if (!GetWorld()->GetTimerManager().IsTimerPaused(TimelineTimerHandle)) {
            GetWorld()->GetTimerManager().PauseTimer(TimelineTimerHandle);
        }

        if (!GetWorld()->GetTimerManager().IsTimerPaused(TimelineNoLoopTimerHandle)) {
            GetWorld()->GetTimerManager().PauseTimer(TimelineNoLoopTimerHandle);
        }
    }
}

FVertexLightInfo AVertexLightingLightAnimated::GetSingleVertexLightInfo()
{
    FVertexLightInfo tempinfo;
    tempinfo.Location = GetActorLocation();
    tempinfo.Radius = InfluenceRadius;
    float MinTime, MaxTime;
    ColorCurve->GetTimeRange(MinTime, MaxTime);
    if (UseLightColorForPreview) {
        tempinfo.LightColor = LightColor;
    }
    else {
        tempinfo.LightColor = ColorCurve->GetLinearColorValue(FMath::Clamp(StartPosition, MinTime, MaxTime));
    };
    tempinfo.Brightness = Brightness;
    tempinfo.InfluenceToNormal = InfluenceToNormal / 100;
    tempinfo.bIsMoveable = IsMovableLight;

    return tempinfo;
}

bool AVertexLightingLightAnimated::SetupTimeline()
{
	if (ColorCurve) {
        FOnTimelineLinearColor ProgressFunction;
        ProgressFunction.BindUFunction(this, FName("OnTimelineUpdate"));
        Timeline.AddInterpLinearColor(ColorCurve, ProgressFunction, FName("ColorTrack"));

        FOnTimelineEvent FinishedFunction;
        FinishedFunction.BindUFunction(this, FName("OnTimelineFinished"));
        Timeline.SetTimelineFinishedFunc(FinishedFunction);

        float MinTime, MaxTime;
        ColorCurve->GetTimeRange(MinTime, MaxTime);

        Timeline.SetTimelineLength(MaxTime);
        Timeline.SetLooping(true);

        Timeline.SetPlaybackPosition(StartAtRandomPosition ? FMath::RandRange(MinTime, MaxTime) : FMath::Clamp(StartPosition, MinTime, MaxTime), false, false);
        Timeline.SetPlayRate(PlayRate/100);

        Timeline.Play();

        return true;
	}
	else {
        #if WITH_EDITOR
		GEngine->AddOnScreenDebugMessage(200, 20.0f, FColor::Orange, "VertextLighting ERROR\n--------------------\n" + this->GetName() + "does not have a valid color curve");
        #endif

		return false;
	}
}

void AVertexLightingLightAnimated::PlayTimeline()
{
    if (!FMath::IsNearlyEqual(CurveTickRate, 0.0f, 0.00001f)) {
        float MinTime, MaxTime;
        ColorCurve->GetTimeRange(MinTime, MaxTime);
        GetWorld()->GetTimerManager().SetTimer(TimelineTimerHandle, this, &AVertexLightingLightAnimated::AdvanceTimeline, CurveTickRate, FTimerManagerTimerParameters{ true, false, -1.0f });

        if (!IsLooping) {
            GetWorld()->GetTimerManager().SetTimer(TimelineNoLoopTimerHandle, this, &AVertexLightingLightAnimated::OnTimelineFinished, MaxTime - FMath::Clamp(StartPosition, MinTime, MaxTime), FTimerManagerTimerParameters{ false, true, -1.0f });
        }
    }
}

void AVertexLightingLightAnimated::StopTimeline()
{
    GetWorldTimerManager().ClearTimer(TimelineTimerHandle);
    UnregisterPreviewLightOnLight();
}

void AVertexLightingLightAnimated::OnTimelineUpdate(FLinearColor Value)
{
    if (GetManager()) {
        FVertexLightInfo info = { GetActorLocation(), InfluenceRadius, Value, Value.A, (InfluenceToNormal/100), IsMovableLight };
        UVertexLightingFunctionLibrary_C::VertexLightAnimated_RegisterUpdate(info, this, Manager, ShouldUpdateNoTickRate);
    }
}

void AVertexLightingLightAnimated::OnTimelineFinished()
{
    if (GetManager()) {
        UVertexLightingFunctionLibrary_C::VertexLight_Unregister(this, Manager);
        Destroy();
    }
}

void AVertexLightingLightAnimated::AdvanceTimeline()
{
    Timeline.TickTimeline(CurveTickRate);
}
