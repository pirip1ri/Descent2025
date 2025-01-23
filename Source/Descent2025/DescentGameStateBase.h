// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DescentGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    StartMenu,
    Playing,
    Paused,
    GameOver,
    GameWin,
    Loading,
    Checkpoint
};

UCLASS()
class DESCENT2025_API ADescentGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    ADescentGameStateBase();

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState CurrentState;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetGameState(EGameState NewState);

    UFUNCTION(BlueprintCallable, Category = "Game State")
    EGameState GetGameState() const { return CurrentState; }
};
