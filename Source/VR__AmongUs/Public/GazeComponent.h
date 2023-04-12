// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GazeComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VR__AMONGUS_API UGazeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGazeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditDefaultsOnly)
		FVector min = FVector(0.05f);

	UPROPERTY(EditDefaultsOnly)
		FVector max = FVector(0.5f);

	bool increase = false;


private:
	//class AHJ_Player* player;
	class AHJ_VR_Player* player;
	float currentSize;
	void ChangeSize(FVector minSize, FVector maxSize);
};
