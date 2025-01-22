#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "DescentPlayerController.generated.h"

/*
 This class is used to control all the player inputs, and have them designate what the pawn will do
 */
UCLASS()
class DESCENT2025_API ADescentPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
private:
    // Input Mapping Context and Input Actions
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveForwardAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* MoveRightAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* LookUpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* TurnAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* CrouchAction;

    // Input Action Functions
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void LookUp(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);
    void StartSprint(const FInputActionValue& Value);
    void StartCrouch(const FInputActionValue& Value);
};