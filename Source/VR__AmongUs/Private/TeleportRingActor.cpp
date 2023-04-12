// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportRingActor.h"
#include "NiagaraComponent.h"

// Sets default values
ATeleportRingActor::ATeleportRingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	niagara_fx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Teleport Ring"));
	niagara_fx->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATeleportRingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATeleportRingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

