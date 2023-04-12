// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PJH_Crew.generated.h"

UCLASS()
class VR__AMONGUS_API APJH_Crew : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APJH_Crew();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSMComponent)
	class UPJH_CrewFSM* fsm;

	FORCEINLINE UPJH_CrewFSM* GetFSM() { return fsm; };

};
