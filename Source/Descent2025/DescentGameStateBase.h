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
    Settings,
    GameOver,
    GameWin,
    Loading,
    Checkpoint,
    None
};

UCLASS()
class DESCENT2025_API ADescentGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    ADescentGameStateBase();

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState PreviousState = EGameState::None;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetPreviousGameState(EGameState NewPreviousState) { PreviousState = NewPreviousState; }

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState CurrentState = EGameState::None;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetGameState(EGameState NewState) { CurrentState = NewState; }

    UFUNCTION(BlueprintCallable, Category = "Game State")
    EGameState GetGameState() const { return CurrentState; }
};
