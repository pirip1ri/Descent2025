// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DescentSaveGame.h"
#include "DescentGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT2025_API UDescentGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UDescentSaveGame* PendingSaveGame;
	UDescentGameInstance();
};
