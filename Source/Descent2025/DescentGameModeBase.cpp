// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

ADescentGameModeBase::ADescentGameModeBase()
{
	// Set the default Game State class as the one we made
	GameStateClass = ADescentGameStateBase::StaticClass();
}

void ADescentGameModeBase::ChangeGameState(EGameState NewState)
{
	ADescentGameStateBase* GameStateRef = GetGameState<ADescentGameStateBase>();
	if (!GameStateRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameStateRef is null in ChangeGameState()!"));
		return;
	}

	// Log the current and new state
	UE_LOG(LogTemp, Display, TEXT("Changing Game State: %d -> %d"), static_cast<int32>(GameStateRef->GetGameState()), static_cast<int32>(NewState));

	// Update the game state, even if it's the same
	GameStateRef->SetGameState(NewState);

	RemoveAllMenusFromViewport();

	// Check what the new game state is, and call the required function
	switch (NewState)
	{
	case EGameState::StartMenu: SetGameToMainMenuMode(); break;
	
	case EGameState::Playing: SetGameToPlay(); break;
	
	case EGameState::Paused: SetGameToPause(); break;
	
	case EGameState::Loading: SetGameToLoad(); break;

	case EGameState::Settings: SetGameToSettings(); break;
	
	case EGameState::GameOver: SetGameToGameOver(); break;
	
	case EGameState::Checkpoint: SetCheckpoint(); break;
	
	case EGameState::GameWin: SetGameToGameWin(); break;
	
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unhandled game state in ChangeGameState!"));
		break;
	}

	// Set the new game state as "Current Game State"
	GameStateRef->SetGameState(NewState);
}

void ADescentGameModeBase::DisplayMainMenuWidget()
{
	// Check there is a main menu template
	if (!MainMenuWidgetTemplate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Main Menu Widget Template is null!"));
		return;
	}

	// Ensure the widget is not already displayed
	if (!MainMenuWidget)
	{
		// If not, set the viewport and controls respectively
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetTemplate);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();

			ADescentPlayerController* PlayerController = GetDescentPlayerController();
			if (PlayerController)
			{
				PlayerController->SetInputMode(FInputModeUIOnly());
				PlayerController->bShowMouseCursor = true;
			}

			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
}

void ADescentGameModeBase::ToggleDisplayPauseMenuWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("void ADescentGameModeBase::ToggleDisplayPauseMenuWidget()"));
	// Check there is a pause menu template
	if (!PauseMenuWidgetTemplate)
	{
		UE_LOG(LogTemp, Display, TEXT("void ADescentGameModeBase::ToggleDisplayPauseMenuWidget() no widget set"));
		return;
	}

	ADescentPlayerController* PlayerController = GetDescentPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADescentGameModeBase::ToggleDisplayPauseMenuWidget() Failed to get PlayerController for ToggleDisplayPauseMenuWidget"));
		return;
	}

	// If you are playing the game currently...
	if (!bIsPaused)
	{
		// Create and display the pause menu
		if (!PauseMenuWidget)
		{
			if (PlayerController)
			{
				PauseMenuWidget = CreateWidget<UUserWidget>(PlayerController, PauseMenuWidgetTemplate);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to get PlayerController for widget ownership!"));
				return;
			}
		}

		if (PauseMenuWidget && !PauseMenuWidget->IsInViewport())
		{
			PauseMenuWidget->AddToViewport();
		}

		// Pause the game and set input mode to UI only
		if (PlayerController)
		{
			PlayerController->SetPause(true);
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
			bIsPaused = true;
		}
	}
	else
	{
		// Remove the pause menu and resume the game
		if (PauseMenuWidget && PauseMenuWidget->IsInViewport())
		{
			PauseMenuWidget->RemoveFromParent();
		}

		if (PlayerController)
		{
			PlayerController->SetPause(false);
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
			bIsPaused = false;
		}

	}
}

void ADescentGameModeBase::DisplayGameOverMenu()
{
	// Retrieve the main menu widget that we have set in the editor, and if it is set, trigger the function
	if (GameOverWidgetTemplate)
	{
		// Create a main menu widget in game, using the template from MainMenuWidgetClass
		GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetTemplate);
		// If successful...
		if (GameOverWidget)
		{
			// Add to viewport
			GameOverWidget->AddToViewport();

			// Set input mode to UI only
			ADescentPlayerController* PlayerController = GetDescentPlayerController();
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				PlayerController->SetInputMode(FInputModeUIOnly());
			}

			// Pause the game
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("AMainMenuGameMode::DisplayMainMenuWidget() no widget set"));
	}
}

void ADescentGameModeBase::DisplaySettingsMenuWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("void ADescentGameModeBase::DisplaySettingsMenuWidget()"));
	// Check there is a pause menu template
	if (!SettingsMenuWidgetTemplate)
	{
		UE_LOG(LogTemp, Display, TEXT("void ADescentGameModeBase::DisplaySettingsMenuWidget() no widget set"));
		return;
	}

	ADescentPlayerController* PlayerController = GetDescentPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("ADescentGameModeBase::DisplaySettingsMenuWidget() Failed to get PlayerController"));
		return;
	}

	// Create and display the settings menu
	if (!SettingsWidget)
	{
		if (PlayerController)
		{
			SettingsWidget = CreateWidget<UUserWidget>(PlayerController, SettingsMenuWidgetTemplate);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to get PlayerController for widget ownership!"));
			return;
		}
	}

	if (SettingsWidget && !SettingsWidget->IsInViewport())
	{
		SettingsWidget->AddToViewport();
	}

	PlayerController->SetPause(true);
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(SettingsWidget->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void ADescentGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Ensure we are setting the correct state based on the loaded level
	FString CurrentLevel = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (CurrentLevel == MainMenuLevelName.ToString())
	{
		ChangeGameState(EGameState::StartMenu);
	}
	else
	{
		ChangeGameState(EGameState::Playing);
	}
}

void ADescentGameModeBase::SetGameToMainMenuMode()
{
	// Go to main menu map

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) != MainMenuLevelName.ToString())
	{
		UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);
	}

	// Delay displaying the menu to ensure the level has loaded
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ADescentGameModeBase::DisplayMainMenuWidget, 0.5f, false);
}

void ADescentGameModeBase::SetGameToPlay()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// Set input mode to Play
	ADescentPlayerController* PlayerController = GetDescentPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("void ADescentGameModeBase::SetGameToPlay() failed to cast the player controller"));
	}
}

void ADescentGameModeBase::SetGameToPause()
{
	UE_LOG(LogTemp, Warning, TEXT("void ADescentGameModeBase::SetGameToPause()"));
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	ADescentPlayerController* PlayerController = GetDescentPlayerController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("void ADescentGameModeBase::SetGameToPause() failed to cast the player controller"));
		return;
	}
	ToggleDisplayPauseMenuWidget();
}

void ADescentGameModeBase::SetGameToSettings()
{
	UE_LOG(LogTemp, Warning, TEXT("void ADescentGameModeBase::SetGameToSettings()"));
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	ADescentPlayerController* PlayerController = GetDescentPlayerController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("void ADescentGameModeBase::SetGameToPause() failed to cast the player controller"));
		return;
	}
	DisplaySettingsMenuWidget();
}

void ADescentGameModeBase::SetGameToLoad()
{
	// Loading mechanics
}

void ADescentGameModeBase::SetGameToGameOver()
{
	// Game Over Mechanics
}

void ADescentGameModeBase::SetGameToGameWin()
{
	// Game Win mechanics
}

void ADescentGameModeBase::SetCheckpoint()
{
	// Checkpoint mechanics
}

ADescentPlayerController* ADescentGameModeBase::GetDescentPlayerController() const
{
	return Cast<ADescentPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void ADescentGameModeBase::RemoveAllMenusFromViewport()
{
	// Store all possible menus in an array
	TArray<UUserWidget*> WidgetsToRemove = { MainMenuWidget, PauseMenuWidget, GameOverWidget, SettingsWidget };

	// Loop through and remove each if it's valid and in the viewport
	for (UUserWidget*& Widget : WidgetsToRemove)
	{
		if (Widget && Widget->IsInViewport())
		{
			Widget->RemoveFromParent();
			Widget = nullptr;
		}
	}

	// Reset any related flags
	bIsPaused = false;
}

void ADescentGameModeBase::StartGame()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
	}

	// Unpause the game
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// Transition to gameplay
	UGameplayStatics::OpenLevel(GetWorld(), "Level1"); /* REPLACE THIS */
}
