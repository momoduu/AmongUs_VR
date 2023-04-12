// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpBoxActor.generated.h"

/**
 * 
 */
UCLASS()
class VR__AMONGUS_API APickUpBoxActor : public APickUpActor
{


	GENERATED_BODY()

public: 
	APickUpBoxActor();
	

		UPROPERTY(EditAnywhere)
		class UBoxComponent* rooComp;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;

};
