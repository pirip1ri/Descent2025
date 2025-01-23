// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ADescentGameModeBase::ADescentGameModeBase()
{
	// Set the default Game State class as the one we made
	GameStateClass = ADescentGameStateBase::StaticClass();
}

void ADescentGameModeBase::ChangeGameState(EGameState NewState)
{
	// Check current Game State
	ADescentGameStateBase* GameStateRef = GetGameState<ADescentGameStateBase>();

	// If the new Game State is the same as the current one, don't do anything, otherwise...
	if (GameStateRef && GameStateRef->GetGameState() != NewState)
	{
		// Set the new Game State as the current game state, and do the required functionality
		
		GameStateRef->SetGameState(NewState);

		switch (NewState)
		{
		case EGameState::StartMenu:
			SetGameToMainMenuMode();
			break;

		case EGameState::Playing:
			SetGameToPlay();
			break;

		case EGameState::Paused:
			SetGameToPause();
			break;

		case EGameState::Loading:
			SetGameToLoad();
			break;

		case EGameState::GameOver:
			SetGameToGameOver();
			break;

		case EGameState::Checkpoint:
			SetCheckpoint();
			break;

		case EGameState::GameWin:
			SetGameToGameWin();
			break;

		default:

			break;
		}
	}
}

void ADescentGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	// ChangeGameState(EGameState::StartMenu);
}

void ADescentGameModeBase::SetGameToMainMenuMode()
{

}

void ADescentGameModeBase::SetGameToPlay()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ADescentGameModeBase::SetGameToPause()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ADescentGameModeBase::SetGameToLoad()
{
}

void ADescentGameModeBase::SetGameToGameOver()
{
}

void ADescentGameModeBase::SetGameToGameWin()
{
}

void ADescentGameModeBase::SetCheckpoint()
{
}
