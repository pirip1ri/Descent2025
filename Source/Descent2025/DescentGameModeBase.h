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
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UUserWidget> FadeToFromBlackWidgetTemplate;
    
    void DisplayMainMenuWidget();
    void ToggleDisplayPauseMenuWidget();
    void DisplayGameOverMenu();
    void DisplaySettingsMenuWidget();
    UFUNCTION(BlueprintCallable, Category = "UI")
    UUserWidget* GetFadeToFromBlackWidget() { return FadeToFromBlackWidget; }

    // Getters
    FVector GetInitialSpawnLocation() const { return InitialSpawnLocation; }
    FRotator GetInitialSpawnRotation() const { return InitialSpawnRotation; }
    FString GetInitialLevelName() const { return InitialLevelName; }

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    float VolumeValue = 1.0f;
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetVolumeValue(float NewVolume) { VolumeValue = NewVolume; }
    UPROPERTY(BlueprintReadOnly, Category = "UI")
    float BrightnessValue = 1.0f;
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetBrightnessValue(float NewBrightness) { BrightnessValue = NewBrightness; }

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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
    FString InitialLevelName = "StartingLevel"; // Set your actual starting level name
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
    UPROPERTY()
    UUserWidget* FadeToFromBlackWidget;

    ADescentPlayerController* GetDescentPlayerController() const;

    void RemoveAllMenusFromViewport();

    void StartGame();
    bool bIsPaused;

    void RegisterInitialSpawn(AActor* PlayerActor);

    FVector InitialSpawnLocation;
    FRotator InitialSpawnRotation;
};
