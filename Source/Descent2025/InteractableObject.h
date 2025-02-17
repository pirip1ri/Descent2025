// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

class UUserWidget;
class UWidgetComponent;

UCLASS()
class DESCENT2025_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void InteractAbility();

	// Interaction Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString InteractionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	FString AlternativeInteractionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	UStaticMeshComponent* StaticMesh;

	// Custom Widget
	UPROPERTY(EditAnywhere, Category = "Components")
	UWidgetComponent* InteractionWidget;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Add a collision component for interaction
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	class UBoxComponent* InteractionBox;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ChangeTextToAlternativeText();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetNewAlternativeText(FString String) { AlternativeInteractionText = String; }
private:
	// Overlap event
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
