// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

// 	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
// 	SetRootComponent(sphereComp);
// 	sphereComp->SetCollisionProfileName(TEXT("PickUp"));
// 
// 	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
// 	meshComp->SetupAttachment(RootComponent);
// 	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

