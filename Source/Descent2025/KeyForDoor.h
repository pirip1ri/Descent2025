// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "KeyForDoor.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT2025_API AKeyForDoor : public AInteractableObject
{
	GENERATED_BODY()
	
	AKeyForDoor();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void InteractAbilityCall();

public:
	UFUNCTION(BlueprintCallable, Category = "Key")
	FString GetKeyName() const { return KeyName; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Key")
	bool bHasBeenCollected = false;

	void Collect();
	void RevertCollect();

	// Unique key name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
	FString KeyName;
};
