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
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VertexLightInfo.h"
#include "VertexLightingLight.generated.h"

class AVertexLightingManagerActor;

UCLASS()
class VERTEXLIGHTINGPLUGIN_API AVertexLightingLight : public AActor
{
	GENERATED_BODY()
	
public:	
	AVertexLightingLight();

	UFUNCTION(BlueprintCallable, Category = "Vertex Light")
	void RegisterPreviewLightOnLight();

	UFUNCTION(BlueprintCallable, Category = "Vertex Light")
	void UnregisterPreviewLightOnLight();

	virtual void EnableActorTicking(bool Enable);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Light")
	virtual FVertexLightInfo GetSingleVertexLightInfo();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Light")
	AVertexLightingManagerActor* GetManager();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Icon")
	void ColorStaticMesh();

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (HideInChildBlueprints = "true", ExposeOnSpawn = "true", ToolTip = "Color of the light. \nOverridden by color curves for `VertexLighting_Light_Animated`"))
	FLinearColor LightColor = FColor::FromHex("FFFFFFFF");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (HideInChildBlueprints = "true", ExposeOnSpawn = "true", Units = "Multiplier", ClampMin = "1.0", ClampMax = "100.0", UIMin = "1.0", UIMax = "100.0", ToolTip = "Light intensity. \nOverridden by color curves for `VertexLighting_Light_Animated`"))
	float Brightness = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (ExposeOnSpawn = "true", Units = "cm", ClampMin = "1.0", UIMin = "1.0", ToolTip = "Radius of the light's influence."))
	float InfluenceRadius = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (ExposeOnSpawn = "true", Units = "%", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0", ToolTip = "Controls how directional the vertex light should be, on a percentage basis. \n0 % makes the light completely non - directional, while 100 % makes it fully directional."))
	float InfluenceToNormal = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (ExposeOnSpawn = "true", Units = "cm", ClampMin = "0.0", ClampMax = "10000.0", UIMin = "0.0", UIMax = "10000.0", ToolTip = "Maximum render distance for the light."))
	float RenderDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (ExposeOnSpawn = "true", ToolTip = "Makes the light's position dynamic during updates."))
	bool IsMovableLight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultSettings", meta = (ExposeOnSpawn = "true", ToolTip = "Determines if the light activates when the player is nearby."))
	bool ShouldTurnOnInRange = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EditorSettings", meta = (ExposeOnSpawn = "true", ToolTip = "Displays the render distance sphere in the editor."))
	bool ShowRenderDistance = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Light_Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* RenderDistanceVisibleSphere_dummy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* Influence_Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* RenderDistance_CheckSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Technical", meta = (ExposeOnSpawn = "true", ToolTip = "The vertex lighting Manager actor that gets searched for by the light.\nDO NOT CHANGE THIS VALUE"))
	TSubclassOf<AVertexLightingManagerActor> VertexLightingManagerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Technical", meta = (ToolTip = "This value memorizes if the player is in-range of this light."))
	bool inPlayerRange = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Technical", meta = (ToolTip = "This value is used to make the light from the start."))
	bool ShouldTickFromStart = false;
	AVertexLightingManagerActor* Manager;
};
