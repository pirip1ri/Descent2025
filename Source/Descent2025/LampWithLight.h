// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PointLightComponent.h"
#include "LampWithLight.generated.h"

UCLASS()
class DESCENT2025_API ALampWithLight : public AActor
{
	GENERATED_BODY()
	
public:
    // Constructor
    ALampWithLight();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Static Mesh for the Lamp
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* LampMesh;

    // Point Light Component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UPointLightComponent* LampLight;

    // Boolean to control whether the light is on or off
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light Settings")
    bool bIsLightOn;

    // Function to toggle the light on/off
    UFUNCTION(BlueprintCallable, Category = "Light Controls")
    void ToggleLight();

    // Function to change the light color from warm to red
    UFUNCTION(BlueprintCallable, Category = "Light Controls")
    void ChangeLightColor();

};
