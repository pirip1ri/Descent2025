// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "DescentPlayerCharacter.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Set default door state
	DoorState = EDoorState::Closed;

	// Setup Door Mesh
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	RootComponent = DoorMesh;
}

void ADoor::InteractAbilityCall()
{
	if (DoorState == EDoorState::Closed) // Prevent re-opening
	{
		if (!bIsLocked)
		{
			// Open door logic (e.g., play animation, disable collision)
			UE_LOG(LogTemp, Warning, TEXT("The door opens!"));
			OpenDoor();
		}
		else
		{
			// Check if the player has the key
			ADescentPlayerCharacter* Player = Cast<ADescentPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if (Player && Player->HasKey(RequiredKey))
			{
				UnlockDoor();
			}
			else
			{
				SetNewAlternativeText("Locked");
				ChangeTextToAlternativeText();

				UE_LOG(LogTemp, Warning, TEXT("The door is locked. You need the key!"));
			}
		}
	}
}

void ADoor::UnlockDoor()
{
	SetNewAlternativeText("Unlocked");

	bIsLocked = false;
	OpenDoor();
}
