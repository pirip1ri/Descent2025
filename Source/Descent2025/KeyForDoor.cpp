// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyForDoor.h"
#include "DescentPlayerCharacter.h"

AKeyForDoor::AKeyForDoor()
{
	InteractionText = TEXT("Key");
}

void AKeyForDoor::InteractAbilityCall()
{
	ADescentPlayerCharacter* Player = Cast<ADescentPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		Player->AddKey(KeyName);
		UE_LOG(LogTemp, Warning, TEXT("Picked up key: %s"), *KeyName);
		Collect();
	}
}

void AKeyForDoor::Collect()
{
	bHasBeenCollected = true;
	SetActorEnableCollision(false); // Disable collision
	StaticMesh->SetVisibility(false);  // Hide the mesh
}

void AKeyForDoor::RevertCollect()
{
	bHasBeenCollected = false;
	SetActorEnableCollision(true); // Disable collision
	StaticMesh->SetVisibility(true);  // Hide the mesh
}
