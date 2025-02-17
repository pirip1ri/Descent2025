// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"
#include "Components/BoxComponent.h"
#include "DescentPlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
    InteractionText = TEXT("Interact");
    AlternativeInteractionText = TEXT("Alternative Text to Interact");

    // Create a root component
    USceneComponent* SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = SceneRoot;

    // Set up the interaction box
    InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
    InteractionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f)); // Adjust size as needed
    InteractionBox->SetCollisionProfileName(TEXT("Trigger"));
    InteractionBox->SetupAttachment(RootComponent);

    // Bind the overlap event
    InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &AInteractableObject::OnOverlapBegin);
    InteractionBox->OnComponentEndOverlap.AddDynamic(this, &AInteractableObject::OnOverlapEnd);

    // Set up the static mesh
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    StaticMesh->SetupAttachment(RootComponent);

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
    UE_LOG(LogTemp, Display, TEXT("AInteractableObject::OnOverlapBegin"));
    ADescentPlayerCharacter* PlayerCharacter = Cast<ADescentPlayerCharacter>(OtherActor);
    if (PlayerCharacter && InteractionWidget)
    {
        UE_LOG(LogTemp, Display, TEXT("AInteractableObject::OnOverlapBegin Widget and Character exist"));
        // Get the widget instance and set the interaction text dynamically
        UUserWidget* WidgetInstance = InteractionWidget->GetUserWidgetObject();
        if (WidgetInstance)
        {
            UE_LOG(LogTemp, Display, TEXT("AInteractableObject::OnOverlapBegin Widget Instance exist"));
            UTextBlock* TextBlock = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("InteractionTextBlock")));
            if (TextBlock)
            {
                UE_LOG(LogTemp, Display, TEXT("AInteractableObject::OnOverlapBegin displayed text"));
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
    // Implementation logic here
}