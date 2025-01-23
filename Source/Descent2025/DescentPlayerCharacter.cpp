// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractableObject.h"
#include "DescentSaveGame.h"
#include <Kismet/GameplayStatics.h>

ADescentPlayerCharacter::ADescentPlayerCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // Create a Camera and attach it to the CameraBoom
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->bUsePawnControlRotation = true;

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCamera);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

    // Set default speeds
    WalkSpeed = 400.f;
    SprintSpeed = 800.f;
    // Set default movement properties
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ADescentPlayerCharacter::MoveForward(float Value)
{
    if (Controller != nullptr)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void ADescentPlayerCharacter::MoveRight(float Value)
{
    if (Controller != nullptr)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void ADescentPlayerCharacter::LookUp(float Value)
{
    if (Controller != nullptr)
    {
        AddControllerPitchInput(Value);
    }
}

void ADescentPlayerCharacter::Turn(float Value)
{
    if (Controller != nullptr)
    {
        AddControllerYawInput(Value);
    }
}

void ADescentPlayerCharacter::StartSprint()
{
    if (Controller != nullptr)
    {
        bIsSprinting = true;
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ADescentPlayerCharacter::StopSprint()
{
    if (Controller != nullptr)
    {
        bIsSprinting = false;
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void ADescentPlayerCharacter::StartCrouch()
{
    if (Controller != nullptr)
    {
        Crouch();
    }
}

void ADescentPlayerCharacter::StopCrouch()
{
    if (Controller != nullptr)
    {
        UnCrouch();
    }
}

void ADescentPlayerCharacter::InteractWithObject()
{
    if (Controller != nullptr)
    {
        // Perform a line trace or check nearby objects
        FHitResult HitResult;
        FVector Start = GetActorLocation(); // Start of the line trace is where the player is
        FVector End = Start + (GetActorForwardVector() * 200.f); // End point = 200m away from Start point in the direction the player is looking in

        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this); // ignore self

        if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params)) // Is there something nearby?
        {
            AInteractableObject* Interactable = Cast<AInteractableObject>(HitResult.GetActor()); // Is that something an interactable object?
            if (Interactable) // If so, interact with it
            {
                Interactable->InteractAbility();
            }
        }
    }
}

// Called when the game starts or when spawned
void ADescentPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void ADescentPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ADescentPlayerCharacter::SaveGame()
{
    // Create an instance of the save
    UDescentSaveGame* SaveGameInstance = Cast<UDescentSaveGame>(UGameplayStatics::CreateSaveGameObject(UDescentSaveGame::StaticClass()));

    // Get information about the player in that instance
    SaveGameInstance->PlayerLocation = GetActorLocation();
    SaveGameInstance->PlayerRotation = GetActorRotation();

    // Create slot for loadout
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, "UpdatedCheckpoint", 0);
}

void ADescentPlayerCharacter::LoadGame()
{
    UDescentSaveGame* LoadGameInstance = Cast<UDescentSaveGame>(UGameplayStatics::LoadGameFromSlot("UpdatedCheckpoint", 0));
    if (LoadGameInstance)
    {
        SetActorLocation(LoadGameInstance->PlayerLocation);
        SetActorRotation(LoadGameInstance->PlayerRotation);
    }
}
