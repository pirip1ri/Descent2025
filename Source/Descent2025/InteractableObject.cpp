// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/SphereComponent.h"
#include "DescentPlayerCharacter.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
    // Set up the interaction sphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->InitSphereRadius(100.f);
    InteractionSphere->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = InteractionSphere;

    // Bind the overlap event
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseInteractable::OnOverlapBegin);

}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ADescentPlayerCharacter* PlayerCharacter = Cast<ADescentPlayerCharacter>(OtherActor);
    if (PlayerCharacter)
    {
        UE_LOG(LogTemp, Display, TEXT("Can Interact with Object!"));
    }
}

void AInteractableObject::InteractAbility_Implementation()
{

}

