// Fill out your copyright notice in the Description page of Project Settings.


#include "PJH_Crew.h"
#include "PJH_CrewFSM.h"

// Sets default values
APJH_Crew::APJH_Crew()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	fsm = CreateDefaultSubobject<UPJH_CrewFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void APJH_Crew::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APJH_Crew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APJH_Crew::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

