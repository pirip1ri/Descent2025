// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DescentGameStateBase.h"
#include "DescentGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DESCENT2025_API ADescentGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    ADescentGameModeBase();

    UFUNCTION(BlueprintCallable, Category = "Game Mode")
    void ChangeGameState(EGameState NewState);

protected:
    virtual void BeginPlay() override;
    void SetGameToMainMenuMode();
    void SetGameToPlay();
    void SetGameToPause();
    void SetGameToLoad();
    void SetGameToGameOver();
    void SetGameToGameWin();
    void SetCheckpoint();
};
