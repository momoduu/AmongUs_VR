// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpActor.generated.h"

UCLASS()
class VR__AMONGUS_API APickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 	UPROPERTY(BluePrintPure)
// 	 FORCEINLINE TObjectPtr<USceneComponent>GetRootComp(){return RootComponent};


protected:
	FName collisionProfileName = FName(TEXT("Pick Up"));


// 
// 	UPROPERTY(EditAnywhere, Category = "PickUp")
// 		class USphereComponent* sphereComp;
// 
// 	UPROPERTY(EditAnywhere, Category = "PickUp")
// 		class UStaticMeshComponent* meshComp;

};
