// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentGameStateBase.h"

ADescentGameStateBase::ADescentGameStateBase()
{
	// Initialize the state to StartMenu
	CurrentState = EGameState::StartMenu;
}
