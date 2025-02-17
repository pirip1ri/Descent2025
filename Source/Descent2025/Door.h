// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "Door.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed     UMETA(DisplayName = "Closed"),
	Opening    UMETA(DisplayName = "Opening"),
	Closing    UMETA(DisplayName = "Closing"),
	Opened     UMETA(DisplayName = "Opened")
};

UCLASS()
class DESCENT2025_API ADoor : public AInteractableObject
{
	GENERATED_BODY()


public:
	ADoor();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractAbilityCall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	EDoorState DoorState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	FString Door;

	FString GetDoorName() const { return Door; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetDoorState(EDoorState NewDoorState) { DoorState = NewDoorState; }
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	EDoorState GetDoorState() { return DoorState; }

	// Is the door locked?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsLocked;

	// Name of the required key to unlock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	FString RequiredKey;

	// Function to unlock the door
	UFUNCTION(BlueprintCallable, Category = "Door")
	void UnlockDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OpenDoor(); 

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void CloseDoor();
};
