// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainUIActor.generated.h"

UCLASS()
class VR__AMONGUS_API AMainUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UWidgetComponent* mainUI;

	UPROPERTY(EditAnywhere)
		class USceneComponent* rootComp;


	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* screenComp;



		void Online_Play();
		void Howto_Play();
		void Exit_Play();


// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
// 		class UWidgetInteractionComponent* WidgetInteractionComp;
};
