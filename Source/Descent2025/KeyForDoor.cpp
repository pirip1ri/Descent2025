// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyForDoor.h"
#include "DescentPlayerCharacter.h"

AKeyForDoor::AKeyForDoor()
{
	InteractionText = TEXT("Key");

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComponent);

	// Load a default mesh (assign a valid asset in Blueprints or via code)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> KeyMeshAsset(TEXT("/Game/Path/To/Your/KeyMesh"));
	if (KeyMeshAsset.Succeeded())
	{
		KeyMesh->SetStaticMesh(KeyMeshAsset.Object);
	}
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
	KeyMesh->SetVisibility(false);  // Hide the mesh
}

void AKeyForDoor::RevertCollect()
{
	bHasBeenCollected = false;
	SetActorEnableCollision(true); // Disable collision
	KeyMesh->SetVisibility(true);  // Hide the mesh
}
