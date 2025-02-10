// Fill out your copyright notice in the Description page of Project Settings.


#include "DescentPlayerCharacter.h"
#include "DescentGameModeBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractableObject.h"
#include "DescentSaveGame.h"
#include "DescentGameInstance.h"
#include "Engine/LevelStreaming.h"
#include <Kismet/GameplayStatics.h>
#include "KeyForDoor.h"
#include "EngineUtils.h"
#include "Door.h"

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
    CurrentHealth = 100.0f;
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

    UDescentGameInstance* MyGameInstance = Cast<UDescentGameInstance>(GetGameInstance());
    if (MyGameInstance && MyGameInstance->PendingSaveGame)
    {
        UE_LOG(LogTemp, Warning, TEXT("Applying saved game data after level reload."));
        ApplySavedData(MyGameInstance->PendingSaveGame);
        MyGameInstance->PendingSaveGame = nullptr; // Prevent reapplying
    }
}

// Called every frame
void ADescentPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ADescentPlayerCharacter::AddKey(FString Key)
{
    PlayerCollectedKeys.Add(Key);
}

bool ADescentPlayerCharacter::HasKey(FString Key) const
{
    return PlayerCollectedKeys.Contains(Key);
}

void ADescentPlayerCharacter::SaveGame()
{
    UDescentSaveGame* SaveGameInstance = Cast<UDescentSaveGame>(UGameplayStatics::CreateSaveGameObject(UDescentSaveGame::StaticClass()));
    if (!SaveGameInstance) return;

    // Store player data
    SaveGameInstance->PlayerLocation = GetActorLocation();
    SaveGameInstance->PlayerRotation = GetActorRotation();

    // Save collected keys
    SaveGameInstance->CollectedKeys.Empty();
    for (TActorIterator<AKeyForDoor> KeyItr(GetWorld()); KeyItr; ++KeyItr)
    {
        if (KeyItr->bHasBeenCollected) // Assume your AKeyForDoor has a boolean tracking if it's collected
        {
            SaveGameInstance->CollectedKeys.Add(KeyItr->GetKeyName());
        }
    }
    // SaveGameInstance->CollectedKeys = PlayerCollectedKeys;
    UE_LOG(LogTemp, Warning, TEXT("Saved %d keys."), PlayerCollectedKeys.Num());

    // Save opened doors
    SaveGameInstance->OpenedDoors.Empty();
    for (TActorIterator<ADoor> DoorItr(GetWorld()); DoorItr; ++DoorItr)
    {
        if (DoorItr->DoorState == EDoorState::Opened) // Assuming you have a boolean tracking if the door is open
        {
            SaveGameInstance->OpenedDoors.Add(DoorItr->GetDoorName());
            UE_LOG(LogTemp, Warning, TEXT("Saved Opened Door: %s"), *DoorItr->GetDoorName());
        }
    }

    // Ensure world is valid
    UWorld* World = GetWorld();
    if (World)
    {
        SaveGameInstance->ActiveSublevels.Empty(); // Clear previous entries

        for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
        {
            if (StreamingLevel && StreamingLevel->IsLevelVisible())
            {
                SaveGameInstance->ActiveSublevels.Add(StreamingLevel->GetWorldAssetPackageName());
            }
        }
    }

    // Save to slot
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, "UpdatedCheckpoint", 0);
}

void ADescentPlayerCharacter::ApplySavedData(UDescentSaveGame* LoadGameInstance)
{
    if (LoadGameInstance)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            // First, unload all existing sublevels
            for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
            {
                if (StreamingLevel)
                {
                    StreamingLevel->SetShouldBeVisible(false);
                }
            }

            // Then, load the saved sublevels
            for (const FString& SublevelName : LoadGameInstance->ActiveSublevels)
            {
                UGameplayStatics::LoadStreamLevel(World, FName(*SublevelName), true, true, FLatentActionInfo());
            }
        }

        // Restore player position and rotation
        SetActorLocation(LoadGameInstance->PlayerLocation);
        SetActorRotation(LoadGameInstance->PlayerRotation);

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->SetControlRotation(LoadGameInstance->PlayerRotation);
        }

        // Restore keys (hide collected keys at time of save)
        PlayerCollectedKeys = LoadGameInstance->CollectedKeys;
        UE_LOG(LogTemp, Warning, TEXT("Loaded %d keys."), PlayerCollectedKeys.Num());

        for (TActorIterator<AKeyForDoor> KeyItr(GetWorld()); KeyItr; ++KeyItr)
        {
            if (LoadGameInstance->CollectedKeys.Contains(KeyItr->GetKeyName()))
            {
                KeyItr->Collect();  // Hide the key
                UE_LOG(LogTemp, Warning, TEXT("Hiding key: %s"), *KeyItr->GetKeyName());
            }
            else
            {
                KeyItr->RevertCollect();  // Show the key
                UE_LOG(LogTemp, Warning, TEXT("Restoring key: %s"), *KeyItr->GetKeyName());
            }
        }



        // Restore doors (close doors that weren't opened)
        for (TActorIterator<ADoor> DoorItr(GetWorld()); DoorItr; ++DoorItr)
        {
            if (LoadGameInstance->OpenedDoors.Contains(DoorItr->GetDoorName()))
            {
                DoorItr->OpenDoor(); // Reopen doors that were opened
                UE_LOG(LogTemp, Warning, TEXT("Reopening Door: %s"), *DoorItr->GetDoorName());
            }
            else
            {
                DoorItr->CloseDoor(); // Close doors that weren’t opened before saving
                UE_LOG(LogTemp, Warning, TEXT("Closing Door: %s"), *DoorItr->GetDoorName());
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No saved game found! Loading initial game level."));

        // Get GameMode to access initial spawn and level
        ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            FString InitialLevelName = GameMode->GetInitialLevelName();
            FString CurrentLevel = GetWorld()->GetMapName();
            CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

            if (InitialLevelName != CurrentLevel)
            {
                UE_LOG(LogTemp, Display, TEXT("Opening initial level: %s"), *InitialLevelName);
                UGameplayStatics::OpenLevel(this, FName(InitialLevelName));
            }
            else
            {
                // Load the necessary sublevels for the initial level
                UWorld* World = GetWorld();
                if (World)
                {
                    for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
                    {
                        if (StreamingLevel)
                        {
                            StreamingLevel->SetShouldBeVisible(true);
                        }
                    }
                }

                // Set player spawn location
                SetActorLocation(GameMode->GetInitialSpawnLocation());
                APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
                if (PlayerController)
                {
                    PlayerController->SetControlRotation(GameMode->GetInitialSpawnRotation());
                }
                SetActorRotation(GameMode->GetInitialSpawnRotation());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GameMode is null! Cannot load initial level."));
        }
    }
}

void ADescentPlayerCharacter::LoadGame()
{
    UDescentSaveGame* LoadGameInstance = Cast<UDescentSaveGame>(UGameplayStatics::LoadGameFromSlot("UpdatedCheckpoint", 0));
    if (LoadGameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Loaded Save Data: Location = %s"), *LoadGameInstance->PlayerLocation.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("LoadGameInstance is NULL!"));
    }

    if (LoadGameInstance)
    {
        // Store the save slot name in the GameInstance so it persists after reloading
        UDescentGameInstance* MyGameInstance = Cast<UDescentGameInstance>(GetGameInstance());
        if (MyGameInstance)
        {
            MyGameInstance->PendingSaveGame = LoadGameInstance;
        }

        ApplySavedData(LoadGameInstance);

        // Reload the level to reset everything first
        /*FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
        UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));*/
    }

    UE_LOG(LogTemp, Warning, TEXT("No saved game found!"));
}
