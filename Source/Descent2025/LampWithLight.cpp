// Fill out your copyright notice in the Description page of Project Settings.


#include "LampWithLight.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"

// Constructor
ALampWithLight::ALampWithLight()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create and attach the static mesh (Lamp model)
    LampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LampMesh"));
    RootComponent = LampMesh;

    // Create and attach the Point Light component
    LampLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LampLight"));
    LampLight->SetupAttachment(RootComponent);

    // Default light settings
    bIsLightOn = true; // Light starts ON by default
    LampLight->SetIntensity(5000.0f);
    LampLight->SetLightColor(FLinearColor::FromSRGBColor(FColor(255, 223, 186))); // Warm light
    LampLight->SetAttenuationRadius(500.f);
}

// Called when the game starts
void ALampWithLight::BeginPlay()
{
    Super::BeginPlay();

    // Apply initial light state from the Details panel
    LampLight->SetVisibility(bIsLightOn);
}

// Toggle Light On/Off
void ALampWithLight::ToggleLight()
{
    bIsLightOn = !bIsLightOn;
    LampLight->SetVisibility(bIsLightOn);
}

// Change Light Color (Warm -> Red, Red -> Warm)
void ALampWithLight::ChangeLightColor()
{
    FLinearColor CurrentColor = LampLight->GetLightColor();
    FLinearColor WarmColor = FLinearColor::FromSRGBColor(FColor(255, 223, 186)); // Warm
    FLinearColor RedColor = FLinearColor::Red; // Red

    // Toggle between warm and red
    if (CurrentColor.Equals(WarmColor, 0.1f))
    {
        LampLight->SetLightColor(RedColor);
    }
    else
    {
        LampLight->SetLightColor(WarmColor);
    }
}

