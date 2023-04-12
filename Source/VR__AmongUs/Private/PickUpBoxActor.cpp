// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpBoxActor.h"
#include "Components/BoxComponent.h"
#include <Components/StaticMeshComponent.h>

APickUpBoxActor::APickUpBoxActor()
{


	rooComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(rooComp);
	rooComp->SetCollisionProfileName(collisionProfileName);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
