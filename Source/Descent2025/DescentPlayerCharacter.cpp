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
    GetCapsuleComponent()->InitCapsuleSize(25.0f, 96.0f);

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
    WalkSpeed = 200.f;
    SprintSpeed = 300.f;
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
        FVector End = Start + (GetActorForwardVector() * 100.f); // End point = 200m away from Start point in the direction the player is looking in

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

    UWorld* World = GetWorld();
    if (!World) return;

    UDescentSaveGame* LoadGameInstance = Cast<UDescentSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SaveSlot"), 0));
    if (LoadGameInstance)
    {
        // Apply saved data (loads saved sublevels)
        ApplySavedData(LoadGameInstance);
    }

    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
    UWorld* World = GetWorld();
    if (World)
    {
        if (LoadGameInstance)
        {
            TArray<FString> SublevelsToLoad;

            if (LoadGameInstance->ActiveSublevels.Num() > 0)
            {
                // Use saved sublevels
                SublevelsToLoad = LoadGameInstance->ActiveSublevels;
            }
            else
            {
                // Use default sublevels from BeginPlay (assuming you stored them in a member variable)
                UE_LOG(LogTemp, Warning, TEXT("No saved sublevels found! Using default ones."));
                // No save file exists, load default sublevels
                TArray<FString> DefaultSublevels = { TEXT("TemplateCarriage"), TEXT("Carriage1") };
                SublevelsToLoad = DefaultSublevels;
            }

            for (ULevelStreaming* StreamingLevel : World->GetStreamingLevels())
            {
                if (StreamingLevel)
                {
                    // Get the sublevel name
                    FString SublevelName = StreamingLevel->GetWorldAssetPackageName();
                    SublevelName.RemoveFromStart(World->StreamingLevelsPrefix);

                    // Check if this sublevel is in the saved ActiveSublevels list
                    bool bShouldLoad = LoadGameInstance->ActiveSublevels.Contains(SublevelName);

                    // If the level needs to be loaded
                    if (bShouldLoad && !StreamingLevel->IsLevelLoaded())
                    {
                        // Log to check level names
                        UE_LOG(LogTemp, Warning, TEXT("Loading Sublevel: %s"), *SublevelName);

                        // Add to the list of levels to check (for future reference)
                        LevelsToCheck.Add(StreamingLevel);

                        // Load the sublevel (no latent action info)
                        UGameplayStatics::LoadStreamLevel(World, FName(*SublevelName), true, true, FLatentActionInfo());

                        if (StreamingLevel && StreamingLevel->IsLevelLoaded())
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Successfully Loaded: %s"), *SublevelName);
                        }
                        else
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Failed to Load or Activate: %s"), *SublevelName);
                        }
                    }
                    // If the level does not need to be loaded (i.e., unload it)
                    else if (!bShouldLoad && StreamingLevel->IsLevelLoaded())
                    {
                        // Log the unloading action
                        UE_LOG(LogTemp, Display, TEXT("Unloading Sublevel: %s"), *SublevelName);

                        // Unload the sublevel asynchronously
                        UGameplayStatics::UnloadStreamLevel(World, FName(*SublevelName), FLatentActionInfo(), false);
                    }
                }
            }
            GetWorld()->GetTimerManager().SetTimer(LoadCheckTimerHandle, this, &ADescentPlayerCharacter::CheckIfLevelsAreLoaded, 0.5f, false);

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
        ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(GetWorld()->GetAuthGameMode());
        FString InitialLevelName = GameMode->GetInitialLevelName();
        UGameplayStatics::OpenLevel(this, FName(InitialLevelName));
        UE_LOG(LogTemp, Display, TEXT("Opening initial level: %s"), *InitialLevelName);
        // Get GameMode to access initial spawn and level
        //ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(GetWorld()->GetAuthGameMode());
        //if (GameMode)
        //{
        //    FString InitialLevelName = GameMode->GetInitialLevelName();
        //    FString CurrentLevel = GetWorld()->GetMapName();
        //    CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

        //    if (InitialLevelName != CurrentLevel)
        //    {
        //        UE_LOG(LogTemp, Display, TEXT("Opening initial level: %s"), *InitialLevelName);
        //        UGameplayStatics::OpenLevel(this, FName(InitialLevelName));
        //    }
        //    else
        //    {
        //        // Force reload the level to ensure everything is set up correctly
        //        UGameplayStatics::OpenLevel(this, FName(*InitialLevelName));

        //        // Set player spawn location
        //        SetActorLocation(GameMode->GetInitialSpawnLocation());
        //        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        //        if (PlayerController)
        //        {
        //            PlayerController->SetControlRotation(GameMode->GetInitialSpawnRotation());
        //        }
        //        SetActorRotation(GameMode->GetInitialSpawnRotation());
        //    }
        //}
    }
}

void ADescentPlayerCharacter::LoadGame()
{
    UDescentSaveGame* LoadGameInstance = Cast<UDescentSaveGame>(UGameplayStatics::LoadGameFromSlot("UpdatedCheckpoint", 0));

    if (LoadGameInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Loaded Save Data: Location = %s"), *LoadGameInstance->PlayerLocation.ToString());
        // Store the save slot name in the GameInstance so it persists after reloading
        UDescentGameInstance* DescentGameInstance = Cast<UDescentGameInstance>(GetGameInstance());
        if (DescentGameInstance)
        {
            DescentGameInstance->PendingSaveGame = LoadGameInstance;
        }

        ApplySavedData(LoadGameInstance);
    }
    else
    {
        ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(GetWorld()->GetAuthGameMode());
        FString InitialLevelName = GameMode->GetInitialLevelName();
        UGameplayStatics::OpenLevel(this, FName(InitialLevelName));
        UE_LOG(LogTemp, Warning, TEXT("No saved game found!"));
        GameMode->FadeFromBlack();
    }
}

void ADescentPlayerCharacter::CheckIfLevelsAreLoaded()
{
    ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(GetWorld()->GetAuthGameMode());
    GameMode->FadeFromBlack();
    
    //bool bAllLoaded = true;
    //UE_LOG(LogTemp, Display, TEXT("Level is loading"));
    //for (ULevelStreaming* Level : LevelsToCheck)
    //{
    //    if (Level && !Level->IsLevelLoaded())
    //    {
    //        bAllLoaded = false;
    //        break;  // Stop checking if any level isn't loaded yet
    //    }
    //}

    //if (bAllLoaded)
    //{
    //    // All levels are now loaded; fade from black
    //    ADescentGameModeBase* GameMode = Cast<ADescentGameModeBase>(GetWorld()->GetAuthGameMode());
    //    GameMode->FadeFromBlack();
    //}
    //else
    //{
    //    // Recheck in 0.1 seconds
    //    GetWorld()->GetTimerManager().SetTimer(LoadCheckTimerHandle, this, &ADescentPlayerCharacter::CheckIfLevelsAreLoaded, 0.1f, false);
    //}
}