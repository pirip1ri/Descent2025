// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DescentGameStateBase.h"
#include "DescentPlayerController.h"
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

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> MainMenuWidgetTemplate;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> PauseMenuWidgetTemplate;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> GameOverWidgetTemplate;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> SettingsMenuWidgetTemplate;
    
    void DisplayMainMenuWidget();
    void ToggleDisplayPauseMenuWidget();
    void DisplayGameOverMenu();
    void DisplaySettingsMenuWidget();

protected:
    virtual void BeginPlay() override;

    void SetGameToMainMenuMode();

    void SetGameToPlay();
    void SetGameToPause();
    void SetGameToSettings();
    void SetGameToLoad();
    void SetGameToGameOver();
    void SetGameToGameWin();
    void SetCheckpoint();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
    FName MainMenuLevelName = "MainMenuLevel";
private:
    /** Reference to the main menu widget */
    UPROPERTY()
    UUserWidget* MainMenuWidget;
    UPROPERTY()
    UUserWidget* PauseMenuWidget;
    UPROPERTY()
    UUserWidget* GameOverWidget;
    UPROPERTY()
    UUserWidget* SettingsWidget;

    ADescentPlayerController* GetDescentPlayerController() const;

    void RemoveAllMenusFromViewport();

    void StartGame();
    bool bIsPaused;
};
