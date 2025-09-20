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
#include "VertexLightingStructure.h"
#include "SimpleTimeStructure.h"
#include "Components/ArrowComponent.h"
#include "VertexLightingPluginSettings.h"

class UTextureRenderTarget2D;
class UVertexLightingPluginSettings;
class UMaterialParameterCollection;
class UCurveLinearColor;
class UClass;
class AVertexLightingLight;

#include "VertexLightingManagerActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMinuteSignature, FSimpleTimeStructure, Time);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHourSignature, FSimpleTimeStructure, Time);

UCLASS()
class VERTEXLIGHTINGPLUGIN_API AVertexLightingManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AVertexLightingManagerActor();

	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting Manager")
	void SetCurrentTime(FSimpleTimeStructure Time);

	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting Manager")
	void ShouldManagerUpdateLights();

	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting Manager")
	void BuildLightsPreview();

	UPROPERTY(BlueprintAssignable, Category = "Vertex Lighting Manager | Time Events")
	FOnMinuteSignature EventOnMinute;

	UPROPERTY(BlueprintAssignable, Category = "Vertex Lighting Manager | Time Events")
	FOnHourSignature EventOnHour;

	// Function to call the dispatchers
	void CallMinuteEvent(const FSimpleTimeStructure& Time);
	void CallHourEvent(const FSimpleTimeStructure& Time);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	//Call In Editor
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Default Settings", meta = (DisplayName = "💡 Draw All Preview Lights"))
	void DrawAllPreviewLights();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Default Settings", meta = (DisplayName = "🛑 Unregister All Preview Lights"))
	void UnregisterAllPreviewLights();

	//Functions
	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting Manager")
	void SetAmbientVertexLight();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Icon")
	void ColorStaticMesh();

private:
	//Functions | Private | Pure
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Vertex Lighting Manager | Private | Pure")
	FSimpleTimeStructure AddTimeToCurrentTime(int32 MinutesToAdd);

	//Functions | Private
	UFUNCTION(BlueprintCallable, Category = "Vertex Lighting Manager | Private")
	bool GetCombinedVertexLightInfo();

	//No interact functions
	UFUNCTION()
	void TickDayNightCycle();

	UFUNCTION()
	void UpdateLights();

	UFUNCTION()
	void ResetUpdateLightsDoOnce();

	UFUNCTION()
	void UpdateConfigSettingValues();

	UFUNCTION()
	void CheckIfRenderTargetIsValid();

	UFUNCTION()
	void CalculateFOVVariables(float FOV);

	UFUNCTION()
	bool DeloadCheck(FVector LightLocation, float LightRadius, FVector CameraLocation, FVector CameraForwardVector, float FOV);

	UFUNCTION()
	void GetPlayerCameraInfo(FVector& CameraLocation, FVector& CameraForwardVector, float& FOV);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//Default Settings | Ambients | Day Night Cycle | Time Events
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Day Night Cycle | Time Events", meta = (ExposeOnSpawn = "true", ToolTip = "Enables the event dispatcher that triggers when the time updates by one minute"))
	bool CallOnMinute = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Day Night Cycle | Time Events", meta = (ExposeOnSpawn = "true", ToolTip = "Enables the event dispatcher that triggers when the time updates by one hour"))
	bool CallOnHour = false;

	//Default Settings | Ambients | Day Night Cycle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Day Night Cycle", meta = (ExposeOnSpawn = "true", ToolTip = "Enables ambient color changes according to the day-night cycle."))
	bool UseDayNightCycleAmbientColor = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Ambients | Day Night Cycle", meta = (ExposeOnSpawn = "true", ToolTip = "Defines the initial time of day using a 24-hour format."))
	FSimpleTimeStructure StartingTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Ambients | Day Night Cycle", meta = (ExposeOnSpawn = "true", ToolTip = "Defines the color transitions for the day-night cycle."))
	UCurveLinearColor* DayNightCycleColorCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Ambients | Day Night Cycle", meta = (ExposeOnSpawn = "true", Units = "s", ToolTip = "Specifies how many real-world seconds equate to one in-game minute."))
	float SecondsPerMinuteInGame = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Ambients | Day Night Cycle", meta = (ExposeOnSpawn = "true", Units = "s", ToolTip = "Time interval between day-night cycle updates.\nSet to `0.0` to disable updates."))
	float TickRateDayNightCycle = 0.25f;

	//Default Settings | Ambients | Ambient
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Ambient", meta = (ExposeOnSpawn = "true", ToolTip = "Default environmental color"))
	FLinearColor AmbientColor = FColor::FromHex("7C7C7CFF");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Ambient", meta = (ExposeOnSpawn = "true", ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0", Units = "%", ToolTip = "Controls how directional the vertex light sunlight should be, on a percentage basis.\n0% makes the sunlight completely non - directional, while 100% makes it fully directional."))
	float SunNormalInfluence = 0.0f;

	//Default Settings | Ambients | Skybox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Skybox", meta = (ExposeOnSpawn = "true", ToolTip = "Adjusts the linked skybox actor’s overall color based on the 'DayNightCycleColorCurve' and time.\nThe linked skybox actor must inherit the 'VertexLighting_Skybox_Interface' for this function to work."))
	bool ChangeSkyboxColorWithDayNightCycle = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Ambients | Skybox", meta = (ExposeOnSpawn = "true", ToolTip = "Specifies the linked skybox actor."))
	AActor* SkyboxActor;

	//Default Settings | Tickrate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Tickrate", meta = (ExposeOnSpawn = "true", ClampMin = "0.0", ClampMax = "60.0", UIMin = "0.0", UIMax = "60.0", Units = "s", ToolTip = "Time interval between light updates.\nSet to `0.0` to disable updates."))
	float TickrateLightUpdate = 0.1f;

	//Default Settings | Optimalization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Optimalization", meta = (ExposeOnSpawn = "true", ClampMin = "0.0", ClampMax = "10000.0", UIMin = "0.0", UIMax = "10000.0", Units = "cm", ToolTip = "Overrides the maximum render distance for all the light.\nSet to `0.0` to disable override."))
	float MaximalLightDistanceOverride = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Optimalization", meta = (ExposeOnSpawn = "true", ToolTip = "This will make any lights unload when hopefully not on screen."))
	bool UnloadOutsideOfView = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Optimalization", meta = (ExposeOnSpawn = "true", ToolTip = "This will make any lights stop ticking when not on screen.\nThis may cause unintended light functioning."))
	bool DisableTickingOffScreen = false;

	//Default Settings | Debug
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Debug", meta = (ExposeOnSpawn = "true", ToolTip = "Enables debug information."))
	bool ShowCombinedVertexLightInfo = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Debug", meta = (ExposeOnSpawn = "true", ToolTip = "Enables debug information."))
	bool ShowVertexLightsRenderedInfo = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Debug", meta = (ExposeOnSpawn = "true", ToolTip = "Enables debug information."))
	bool ShowDayNightTimeDebugInfo = true;

	UPROPERTY(BlueprintReadOnly, Category = "Logic")
	TMap<AVertexLightingLight*, FVertexLightingStructure> VertexLightArray;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Manager_Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SunDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Technical", meta = (ExposeOnSpawn = "true", ToolTip = "This is the Material Parameter Collection the code uses.\nDO NOT CHANGE THIS VALUE!"))
	UMaterialParameterCollection* collection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Settings | Technical", meta = (ExposeOnSpawn = "true", ToolTip = "These are the parameter names the code uses.\nDO NOT CHANGE THIS VALUE!"))
	TArray<FName> collectionParameterNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Technical", meta = (ExposeOnSpawn = "true", ToolTip = "The current time variable used for the time.\nDO NOT CHANGE THIS VALUE!"))
	FSimpleTimeStructure CurrentTime = { 0,0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Technical", meta = (ExposeOnSpawn = "true", ToolTip = "The vertex lighting light actor that gets searched for by the manager.\nDO NOT CHANGE THIS VALUE!"))
	UClass* VertexLightingLightActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Settings | Technical", meta = (ExposeOnSpawn = "true", ToolTip = "The render target texture that gets used.\nDO NOT CHANGE THIS VALUE!"))
	UTextureRenderTarget2D* VertexLightData;

private:
	UPROPERTY()
	int32 RenderTarget_PixelCounter = 0;

	UPROPERTY()
	FSimpleTimeStructure PreviousTime = { 0,0 };

	UPROPERTY()
	TArray<FLinearColor> VertexLights_Array_Loc;

	UPROPERTY()
	TArray<FLinearColor> VertexLights_Array_Color;

	UPROPERTY() //True means it can go thru
	bool DoOnceBool = true;

	UPROPERTY()
	bool WaitingForTick = false;

	UPROPERTY() 
	bool BeginplayDoOnce = true;

	UPROPERTY()
	FTimerHandle DayNightTimerHandle;

	UPROPERTY()
	FTimerHandle TickTimerHandle;

	UPROPERTY()
	float SavedAlmostZero;

	UPROPERTY()
	int32 wantedRenderTextureSize;

	UPROPERTY()
	int32 MaxLights;

	UPROPERTY()
	UVertexLightingPluginSettings* ConfigSettings;

	UPROPERTY()
	float HalfFOVRad = 0.0f;
};
