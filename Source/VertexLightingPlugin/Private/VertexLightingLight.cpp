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

#include "VertexLightingLight.h"
#include "Kismet/GameplayStatics.h"
#include "VertexLightingFunctionLibrary_C.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

AVertexLightingLight::AVertexLightingLight()
{
	PrimaryActorTick.bCanEverTick = ShouldTickFromStart;
    bGenerateOverlapEventsDuringLevelStreaming = true;

    Light_Icon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Light_Icon"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
    if (PlaneMeshAsset.Succeeded())
    {
        Light_Icon->SetStaticMesh(PlaneMeshAsset.Object);
        Light_Icon->SetMaterial(0, LoadObject<UMaterialInterface>(nullptr, TEXT("/VertexLightingPlugin/Icons/Materials/VertexLighting_Icon_Light.VertexLighting_Icon_Light")));
    }

    Light_Icon->SetCollisionProfileName("NoCollision", false);
    Light_Icon->SetComponentTickEnabled(false);
    Light_Icon->SetGenerateOverlapEvents(false);
    Light_Icon->SetCastShadow(false);
    Light_Icon->bAffectDynamicIndirectLighting = false;
    Light_Icon->bAffectDistanceFieldLighting = false;
    Light_Icon->bHiddenInGame = true;
    Light_Icon->bWorldPositionOffsetWritesVelocity = false;
    Light_Icon->bVisibleInReflectionCaptures = false;
    Light_Icon->bVisibleInRealTimeSkyCaptures = false;
    Light_Icon->bVisibleInRayTracing = false;
    Light_Icon->bReceivesDecals = false;
    Light_Icon->bUseAsOccluder = false;
    Light_Icon->bReceiveMobileCSMShadows = false;
    Light_Icon->SetCanEverAffectNavigation(false);

    RenderDistanceVisibleSphere_dummy = CreateDefaultSubobject<USphereComponent>(TEXT("RenderDistanceVisibleSphere_dummy"));
    RenderDistanceVisibleSphere_dummy->SetupAttachment(Light_Icon);
    RenderDistanceVisibleSphere_dummy->SetSphereRadius(RenderDistance);
    RenderDistanceVisibleSphere_dummy->bReceiveMobileCSMShadows = false;
    RenderDistanceVisibleSphere_dummy->SetCollisionProfileName("NoCollision", false);
    RenderDistanceVisibleSphere_dummy->SetComponentTickEnabled(false);
    RenderDistanceVisibleSphere_dummy->SetGenerateOverlapEvents(false);
    RenderDistanceVisibleSphere_dummy->bVisibleInReflectionCaptures = false;
    RenderDistanceVisibleSphere_dummy->bVisibleInRealTimeSkyCaptures = false;
    RenderDistanceVisibleSphere_dummy->bVisibleInRayTracing = false;
    RenderDistanceVisibleSphere_dummy->bReceivesDecals = false;
    RenderDistanceVisibleSphere_dummy->bUseAsOccluder = false;
    RenderDistanceVisibleSphere_dummy->bAffectDynamicIndirectLighting = false;
    RenderDistanceVisibleSphere_dummy->bAffectDistanceFieldLighting = false;
    RenderDistanceVisibleSphere_dummy->bHiddenInGame = true;
    RenderDistanceVisibleSphere_dummy->SetVisibility(false);
    RenderDistanceVisibleSphere_dummy->SetCanEverAffectNavigation(false);

    Influence_Radius = CreateDefaultSubobject<USphereComponent>(TEXT("Influence_Radius"));
    Influence_Radius->SetupAttachment(Light_Icon);
    Influence_Radius->SetSphereRadius(InfluenceRadius);
    Influence_Radius->bReceiveMobileCSMShadows = false;
    Influence_Radius->SetCollisionProfileName("NoCollision", false);
    Influence_Radius->SetComponentTickEnabled(false);
    Influence_Radius->SetGenerateOverlapEvents(false);
    Influence_Radius->bVisibleInReflectionCaptures = false;
    Influence_Radius->bVisibleInRealTimeSkyCaptures = false;
    Influence_Radius->bVisibleInRayTracing = false;
    Influence_Radius->bReceivesDecals = false;
    Influence_Radius->bUseAsOccluder = false;
    Influence_Radius->bAffectDynamicIndirectLighting = false;
    Influence_Radius->bAffectDistanceFieldLighting = false;
    Influence_Radius->bHiddenInGame = true;
    Influence_Radius->SetCanEverAffectNavigation(false);

    RenderDistance_CheckSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RenderDistance_CheckSphere"));
    RenderDistance_CheckSphere->SetupAttachment(Light_Icon);
    RenderDistance_CheckSphere->bReceiveMobileCSMShadows = false;
    RenderDistance_CheckSphere->SetCollisionProfileName("OverlapOnlyPawn", true);
    RenderDistance_CheckSphere->SetComponentTickEnabled(true);
    RenderDistance_CheckSphere->SetGenerateOverlapEvents(true);
    RenderDistance_CheckSphere->bVisibleInReflectionCaptures = false;
    RenderDistance_CheckSphere->bVisibleInRealTimeSkyCaptures = false;
    RenderDistance_CheckSphere->bVisibleInRayTracing = false;
    RenderDistance_CheckSphere->bReceivesDecals = false;
    RenderDistance_CheckSphere->bUseAsOccluder = false;
    RenderDistance_CheckSphere->bAffectDynamicIndirectLighting = false;
    RenderDistance_CheckSphere->bAffectDistanceFieldLighting = false;
    RenderDistance_CheckSphere->bHiddenInGame = true;
    RenderDistance_CheckSphere->SetVisibility(false);
    RenderDistance_CheckSphere->bAffectDynamicIndirectLighting = false;
    RenderDistance_CheckSphere->SetCanEverAffectNavigation(false);

    VertexLightingManagerActor = StaticLoadClass(AVertexLightingManagerActor::StaticClass(), nullptr, TEXT("/VertexLightingPlugin/VertexLighting_Manager.VertexLighting_Manager_C"));
}

// Called when the game starts or when spawned
void AVertexLightingLight::BeginPlay()
{
	Super::BeginPlay();
    RenderDistance_CheckSphere->SetSphereRadius(RenderDistance, true);

    if (!inPlayerRange) {
        if (GetManager()) {
            UVertexLightingFunctionLibrary_C::VertexLight_Unregister(this, Manager);
        }
    }
}

FVertexLightInfo AVertexLightingLight::GetSingleVertexLightInfo()
{
    FVertexLightInfo tempinfo;
    tempinfo.Location = GetActorLocation();
    tempinfo.Radius = InfluenceRadius;
    tempinfo.LightColor = LightColor;
    tempinfo.Brightness = Brightness;
    tempinfo.InfluenceToNormal = InfluenceToNormal / 100;
    tempinfo.bIsMoveable = IsMovableLight;

    return tempinfo;
}

void AVertexLightingLight::RegisterPreviewLightOnLight()
{
    if (GetManager()) {
        UVertexLightingFunctionLibrary_C::VertexLight_Register(GetSingleVertexLightInfo(), this, Manager);

        if (GIsEditor && !GIsPlayInEditorWorld)
        {
            Manager->BuildLightsPreview();
        }
    }
    else {
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(800, 20.0f, FColor::Red, "Vertex Lighting ERROR\n--------------------\nFrom VertexLightingLight.cpp\n--------------------\nNo VertexLighting_Manager found");
        #endif
    }
}

void AVertexLightingLight::UnregisterPreviewLightOnLight()
{
    if (GetManager()) {
        UVertexLightingFunctionLibrary_C::VertexLight_Unregister(this, Manager);
        Manager->BuildLightsPreview();
    }
    else {
        #if WITH_EDITOR
        GEngine->AddOnScreenDebugMessage(800, 20.0f, FColor::Red, "Vertex Lighting ERROR\n--------------------\nNo VertexLighting_Manager found");
        #endif
    }
}

void AVertexLightingLight::EnableActorTicking(bool Enable)
{
    //Do nothing if this is a normal light
}

void AVertexLightingLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AVertexLightingManagerActor* AVertexLightingLight::GetManager() {
    if (!Manager) {
        if (VertexLightingManagerActor) {
            Manager = Cast<AVertexLightingManagerActor>(UGameplayStatics::GetActorOfClass(this, VertexLightingManagerActor));
        }
    }
    return Manager;
}

