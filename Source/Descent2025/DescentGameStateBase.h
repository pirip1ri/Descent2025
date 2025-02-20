// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DescentGameStateBase.generated.h"

UENUM(BlueprintType)
enum class EDescentGameState : uint8
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
    EDescentGameState PreviousState = EDescentGameState::None;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetPreviousGameState(EDescentGameState NewPreviousState) { PreviousState = NewPreviousState; }

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EDescentGameState CurrentState = EDescentGameState::None;

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void SetGameState(EDescentGameState NewState) { CurrentState = NewState; }

    UFUNCTION(BlueprintCallable, Category = "Game State")
    EDescentGameState GetGameState() const { return CurrentState; }
};
