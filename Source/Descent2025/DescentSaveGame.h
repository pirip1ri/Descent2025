// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DescentSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT2025_API UDescentSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	FVector PlayerLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	FRotator PlayerRotation;

	UPROPERTY(BlueprintReadWrite, Category = "Save Data")
	int32 PlayerHealth;
};
