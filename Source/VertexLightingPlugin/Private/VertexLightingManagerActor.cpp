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

#include "VertexLightingManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "VertexLightingLight.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Curves/CurveLinearColor.h"
#include "VertexLightingFunctionLibrary_C.h"
#include "VertexLightingSkyboxInterface.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "MeshVertexPainter/MeshVertexPainterKismetLibrary.h"
#include "VertexLightingLightAnimated.h"

AVertexLightingManagerActor::AVertexLightingManagerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Manager_Icon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Manager_Icon"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMeshAsset.Succeeded())
	{
		Manager_Icon->SetStaticMesh(PlaneMeshAsset.Object);
	}

	SunDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("SunDirection"));
	SunDirection->SetupAttachment(Manager_Icon);
	SunDirection->ArrowColor = FColor(255, 222, 0, 255);
	SunDirection->ArrowSize = 2.0f;
	SunDirection->ArrowLength = 50.0f;

	ConfigSettings = Cast<UVertexLightingPluginSettings>(UVertexLightingPluginSettings::StaticClass()->GetDefaultObject());
	UpdateConfigSettingValues();

	StartingTime = { 7, 0 };
	CurrentTime = StartingTime;

	collectionParameterNames = { FName("DefaultColorAndBrightness"), FName("SunDirection") , FName("NumActiveLights") };
}

void AVertexLightingManagerActor::SetCurrentTime(FSimpleTimeStructure Time)
{
	CurrentTime = Time;
	SetAmbientVertexLight();

	if ((CurrentTime.Hours != PreviousTime.Hours) && CallOnHour) {
		this->CallHourEvent(CurrentTime);
	}

	if ((CurrentTime.Minutes != PreviousTime.Minutes) && CallOnMinute) {
		this->CallMinuteEvent(CurrentTime);
	}

	PreviousTime = CurrentTime;

	if (ChangeSkyboxColorWithDayNightCycle && SkyboxActor) {
		if (SkyboxActor->GetClass()->ImplementsInterface(UVertexLightingSkyboxInterface::StaticClass())) {
			IVertexLightingSkyboxInterface::Execute_UpdateDayNightTime(SkyboxActor, CurrentTime, DayNightCycleColorCurve);
		}
	}
}

void AVertexLightingManagerActor::BeginPlay()
{
	Super::BeginPlay();

	if (BeginplayDoOnce) {
		BeginplayDoOnce = false;

		CheckIfRenderTargetIsValid();

		SetAmbientVertexLight();

		UWorld* World_Ref = GetWorld();

		if (UseDayNightCycleAmbientColor && (!FMath::IsNearlyEqual(TickRateDayNightCycle, 0.0f, 0.00001f))) {
			World_Ref->GetTimerManager().SetTimer(DayNightTimerHandle, this, &AVertexLightingManagerActor::TickDayNightCycle, TickRateDayNightCycle, FTimerManagerTimerParameters{ true, false, -1.0f });
		}

		if (FMath::IsNearlyEqual(TickrateLightUpdate, 0.0f, 0.00001f)) {
			SetActorTickInterval(0.0f);
		}
		else {
			World_Ref->GetTimerManager().SetTimer(TickTimerHandle, this, &AVertexLightingManagerActor::UpdateLights, TickrateLightUpdate, FTimerManagerTimerParameters{ true, false, -1.0f });
			SetActorTickInterval(std::min(TickrateLightUpdate, TickRateDayNightCycle));
		}
	}
}

void AVertexLightingManagerActor::DrawAllPreviewLights()
{
	if (VertexLightingLightActor) {
		TArray <AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(this, AVertexLightingLight::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors) {
			if (Actor) {
				AVertexLightingLight* LightActor = Cast<AVertexLightingLight>(Actor);
				if (LightActor)
				{
					LightActor->RegisterPreviewLightOnLight();
				}
			}
		}
	}
}

void AVertexLightingManagerActor::UnregisterAllPreviewLights()
{
	VertexLightArray.Empty();
	BuildLightsPreview();
}

void AVertexLightingManagerActor::SetAmbientVertexLight()
{
	if (collection && collectionParameterNames.Num() == 2 && DayNightCycleColorCurve) {
		UWorld* World_Ref = GetWorld();

		const FLinearColor& data = UseDayNightCycleAmbientColor ? DayNightCycleColorCurve->GetLinearColorValue(UVertexLightingFunctionLibrary_C::TimeToPercentage(CurrentTime)) : AmbientColor;
		UKismetMaterialLibrary::SetVectorParameterValue(World_Ref, collection, collectionParameterNames[0], data);

		FVector forwardVector = GetActorForwardVector() * -1;
		UKismetMaterialLibrary::SetVectorParameterValue(World_Ref, collection, collectionParameterNames[1], FLinearColor(forwardVector.X, forwardVector.Y, forwardVector.Z, (SunNormalInfluence/100)));
	}
}

void AVertexLightingManagerActor::ShouldManagerUpdateLights()
{
	if (FMath::IsNearlyEqual(TickrateLightUpdate, 0.0f, 0.00001f)) {
		UpdateLights();
	}
}

FSimpleTimeStructure AVertexLightingManagerActor::AddTimeToCurrentTime(int32 MinutesToAdd)
{
	int32 HoursToAdd = FMath::FloorToInt(float(MinutesToAdd)/60);

	int32 tempMinutes = CurrentTime.Minutes + (MinutesToAdd - (HoursToAdd * 60));
	int32 tempHours = (CurrentTime.Hours + HoursToAdd) % 24;

	tempHours = (tempHours + FMath::FloorToInt(float(tempMinutes) / 60)) % 24;
	tempMinutes = tempMinutes % 60;

	return { (uint8)tempHours , (uint8)tempMinutes };
}

bool AVertexLightingManagerActor::GetCombinedVertexLightInfo()
{
	if (ShowCombinedVertexLightInfo && VertexLightData) {
		int32 Lightnum = VertexLightArray.Num();
		int32 LightDataXSize = MaxLights;
		if (Lightnum < LightDataXSize) {
			FColor debugColor;
			if (Lightnum < (LightDataXSize - (LightDataXSize / 4))) {
				debugColor = FColor::Green;
			}
			else {
				debugColor = FColor::Yellow;
			}

			#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(800, 20.0f, debugColor, "Vertex Lighting Information\n--------------------\nFrom VertexLightingManagerActor.cpp\n--------------------\nAmount of Vertex Lights Registered: " + FString::FromInt(Lightnum));
			#endif

			return true;
		}
		else {
			#if WITH_EDITOR
			GEngine->AddOnScreenDebugMessage(800, 20.0f, FColor::Red, "Vertex Lighting ERROR\n--------------------\nFrom VertexLightingManagerActor.cpp\n--------------------\nToo Many Vertex Lights Registered");
			#endif

			return false;
		}
	}
	return false;
}

void AVertexLightingManagerActor::TickDayNightCycle()
{
	float tempFloat = TickRateDayNightCycle / SecondsPerMinuteInGame;
	int32 tempInt = FMath::Floor(tempFloat);

	if (tempInt == 0) {
		SavedAlmostZero += tempFloat;
		if ((SavedAlmostZero + tempFloat) > 1.0f) {
			SetCurrentTime(AddTimeToCurrentTime(SavedAlmostZero + tempFloat));
			SavedAlmostZero = 0.0f;
		}
		else {
			SetCurrentTime(AddTimeToCurrentTime(tempInt));
		}
	}
	else {
		SavedAlmostZero = 0.0f;
		SetCurrentTime(AddTimeToCurrentTime(tempInt));
	}

	#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(805, ShowDayNightTimeDebugInfo ? 20.0f : 0.0f, FColor::Blue, "Vertex Lighting Information\n--------------------\nDay Night Cycle Time: " + FString::FromInt(CurrentTime.Hours) + " : " + FString::FromInt(CurrentTime.Minutes));
	#endif
}

void AVertexLightingManagerActor::UpdateLights()
{
	if (DoOnceBool) {
		DoOnceBool = false;
		BuildLightsPreview();
	}
}

void AVertexLightingManagerActor::ResetUpdateLightsDoOnce()
{
	if (!WaitingForTick) {
		WaitingForTick = true;
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
			{
				DoOnceBool = true;
				WaitingForTick = false;
			});
	}
}

void AVertexLightingManagerActor::UpdateConfigSettingValues()
{
	if (ConfigSettings)
	{
		MaxLights = ConfigSettings->MaxLights;
		wantedRenderTextureSize = UVertexLightingFunctionLibrary_C::GetVertexLightTextureDataSize(MaxLights);
	}
}

void AVertexLightingManagerActor::CheckIfRenderTargetIsValid()
{
	if (IsValid(VertexLightData)) {
		if (!UVertexLightingFunctionLibrary_C::IsRenderTargetRightSize(VertexLightData, MaxLights)) {
			VertexLightData->InitCustomFormat(wantedRenderTextureSize, wantedRenderTextureSize, PF_FloatRGBA, false);
			VertexLightData->UpdateResourceImmediate(true);
		}
	}
}

bool AVertexLightingManagerActor::DeloadCheck(FVector LightLocation, float LightRadius, FVector CameraLocation, FVector CameraForwardVector, float FOV)
{
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		return true;
	}

	const float Distance = FVector::Dist(LightLocation, CameraLocation);

	if (MaximalLightDistanceOverride > 0.001f && Distance > MaximalLightDistanceOverride)
	{
		return false;
	}

	if (Distance < LightRadius)
	{
		return true;
	}

	if (UnloadOutsideOfView)
	{
		const FVector ToLight = (LightLocation - CameraLocation).GetSafeNormal(0.0001f);

		const float CosAngle = FVector::DotProduct(ToLight, CameraForwardVector);

		const float ExpandedAngle = LightRadius / Distance;
		const float EffectiveHalfFOV = HalfFOVRad + ExpandedAngle;

		const float CosThreshold = FMath::Cos(EffectiveHalfFOV);

		if (CosAngle < CosThreshold)
		{
			return false;
		}
	}

	return true;
}

void AVertexLightingManagerActor::CalculateFOVVariables(float FOV) {
	HalfFOVRad = FMath::DegreesToRadians(FOV * 0.6f);
}

void AVertexLightingManagerActor::GetPlayerCameraInfo(FVector& CameraLocation, FVector& CameraForwardVector, float& FOV)
{
	APlayerController* playercontroller = UGameplayStatics::GetPlayerController(this, 0);

	if (playercontroller) {
		APlayerCameraManager* manager = playercontroller->PlayerCameraManager;

		CameraLocation = manager->GetCameraLocation();
		CameraForwardVector = manager->GetActorForwardVector();
		FOV = manager->GetFOVAngle();
	}
	else {
		CameraLocation = FVector(0, 0, 0);
		CameraForwardVector = FVector(0, 0, 0);
		FOV = 0.0f;
	}
}

void AVertexLightingManagerActor::BuildLightsPreview()
{
	RenderTarget_PixelCounter = 0;
	VertexLights_Array_Loc.Empty();
	VertexLights_Array_Color.Empty();

	if (!GetCombinedVertexLightInfo()) {
		return;
	}

	FVector CameraLocation;
	FVector CameraForwardVector;
	float FOV;
	GetPlayerCameraInfo(CameraLocation, CameraForwardVector, FOV);
	CalculateFOVVariables(FOV);

	for (auto& Pair : VertexLightArray)
	{
		if (!IsValid(Pair.Key)) {
			continue;
		}

		FVector finalLocation = Pair.Value.IsMoveableLight ? UVertexLightingFunctionLibrary_C::LocationToVertexLightLocation(Pair.Key->GetActorLocation()) : FVector(Pair.Value.LocationAndRadius.X, Pair.Value.LocationAndRadius.Y, Pair.Value.LocationAndRadius.Z);

		FVector LightLocation = Pair.Value.IsMoveableLight ? UVertexLightingFunctionLibrary_C::LocationToVertexLightLocation(Pair.Key->GetActorLocation()) : FVector(Pair.Value.LocationAndRadius) * 100;

		if (DeloadCheck(LightLocation, Pair.Value.LocationAndRadius.W, CameraLocation, CameraForwardVector, FOV))
		{
			VertexLights_Array_Loc.Add(FLinearColor(finalLocation.X, finalLocation.Y, finalLocation.Z, Pair.Value.LocationAndRadius.W));
			VertexLights_Array_Color.Add(Pair.Value.ColorAndInfluenceOnNormal);

			if (DisableTickingOffScreen) {
				Pair.Key->EnableActorTicking(true);
			}
		}
		else if (DisableTickingOffScreen)
		{
			Pair.Key->EnableActorTicking(false);
		}
	}

	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, collectionParameterNames[2], VertexLights_Array_Loc.Num());
	UVertexLightingFunctionLibrary_C::DrawVertexLightInformationToRenderTarget(VertexLightData, VertexLights_Array_Loc, VertexLights_Array_Color);

	#if WITH_EDITOR
	if (ShowVertexLightsRenderedInfo)
	{
		GEngine->AddOnScreenDebugMessage(820, 20.0f, FColor::Emerald, FString::Printf(TEXT("Vertex Lights rendered: %d"), VertexLights_Array_Loc.Num()));
	}
	#endif

	ResetUpdateLightsDoOnce();

}

void AVertexLightingManagerActor::CallMinuteEvent(const FSimpleTimeStructure& Time)
{
	EventOnMinute.Broadcast(Time);
}

void AVertexLightingManagerActor::CallHourEvent(const FSimpleTimeStructure& Time)
{
	EventOnHour.Broadcast(Time);
}

void AVertexLightingManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR
void AVertexLightingManagerActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UpdateConfigSettingValues();
}
#endif
