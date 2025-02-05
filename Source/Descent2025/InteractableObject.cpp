// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/SphereComponent.h"
#include "DescentPlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
    InteractionText = TEXT("Interact");
    AlternativeInteractionText = TEXT("Alternative Text to Interact");

    // Set up the interaction sphere
    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->InitSphereRadius(100.f);
    InteractionSphere->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = InteractionSphere;

    // Bind the overlap event
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnOverlapBegin);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnOverlapEnd);

    // Set up the interaction widget
    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);
    InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen); // Keeps it on screen
    InteractionWidget->SetDrawSize(FVector2D(200.f, 50.f)); // Adjust size as needed
    InteractionWidget->SetVisibility(false); // Initially hidden
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();

    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(false);
    }
}

void AInteractableObject::ChangeTextToAlternativeText()
{
   if (InteractionWidget)
    {
        // Get the widget instance and set the interaction text dynamically
        UUserWidget* WidgetInstance = InteractionWidget->GetUserWidgetObject();
        if (WidgetInstance)
        {
            UTextBlock* TextBlock = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("InteractionTextBlock")));
            if (TextBlock)
            {
                TextBlock->SetText(FText::FromString(AlternativeInteractionText));
            }
        }

        InteractionWidget->SetVisibility(true);
    }
}

void AInteractableObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ADescentPlayerCharacter* PlayerCharacter = Cast<ADescentPlayerCharacter>(OtherActor);
    if (PlayerCharacter && InteractionWidget)
    {
        // Get the widget instance and set the interaction text dynamically
        UUserWidget* WidgetInstance = InteractionWidget->GetUserWidgetObject();
        if (WidgetInstance)
        {
            UTextBlock* TextBlock = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("InteractionTextBlock")));
            if (TextBlock)
            {
                TextBlock->SetText(FText::FromString(InteractionText));
            }
        }

        InteractionWidget->SetVisibility(true);
    }
}

void AInteractableObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ADescentPlayerCharacter* PlayerCharacter = Cast<ADescentPlayerCharacter>(OtherActor);
    if (PlayerCharacter && InteractionWidget)
    {
        InteractionWidget->SetVisibility(false);
    }
}

void AInteractableObject::InteractAbility_Implementation()
{

}

