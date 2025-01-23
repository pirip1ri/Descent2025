// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentPlayerController.h"
#include "DescentPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ADescentPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Add the IMC
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void ADescentPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Enhanced Input Component
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // Bind Movement
        EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ADescentPlayerController::MoveForward);
        EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ADescentPlayerController::MoveRight);

        // Bind Looking
        EnhancedInput->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &ADescentPlayerController::LookUp);
        EnhancedInput->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ADescentPlayerController::Turn);

        // Bind Sprint
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ADescentPlayerController::StartSprint);
        EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADescentPlayerController::StartSprint);

        // Bind Crouch
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADescentPlayerController::StartCrouch);
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ADescentPlayerController::StartCrouch);

        // Bind Interact
        EnhancedInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADescentPlayerController::InteractWithObject);
    }
}



void ADescentPlayerController::MoveForward(const FInputActionValue& Value)
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        float ForwardValue = Value.Get<float>(); // Get the input value as a float
        DescentCharacter->MoveForward(ForwardValue); // Call ""
    }
}

void ADescentPlayerController::MoveRight(const FInputActionValue& Value)
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        float RightValue = Value.Get<float>();
        DescentCharacter->MoveRight(RightValue);
    }
}

void ADescentPlayerController::LookUp(const FInputActionValue& Value)
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        float LookUpValue = Value.Get<float>();
        DescentCharacter->LookUp(LookUpValue);
    }
}

void ADescentPlayerController::Turn(const FInputActionValue& Value)
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        float TurnValue = Value.Get<float>();
        DescentCharacter->Turn(TurnValue);
    }
}

void ADescentPlayerController::StartSprint(const FInputActionValue& Value)
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        if (Value.Get<bool>())
        {
            DescentCharacter->StartSprint();
        }
        else
        {
            DescentCharacter->StopSprint();
        }
    }
}

void ADescentPlayerController::StartCrouch(const FInputActionValue& Value)
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        if (Value.Get<bool>())
        {
            DescentCharacter->StartCrouch();
        }
        else
        {
            DescentCharacter->StopCrouch();
        }
    }
}

void ADescentPlayerController::InteractWithObject()
{
    if (ADescentPlayerCharacter* DescentCharacter = Cast<ADescentPlayerCharacter>(GetPawn()))
    {
        DescentCharacter->InteractWithObject();
    }
}
