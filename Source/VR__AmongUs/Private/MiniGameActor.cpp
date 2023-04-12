// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGameActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>

// Sets default values
AMiniGameActor::AMiniGameActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		//rooComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	// 	SetRootComponent(rooComp);
	// 	rooComp->SetCollisionProfileName(collisionProfileName);
	// 
	// 	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	// 	meshComp->SetupAttachment(RootComponent);
	// 	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("UIInteraction Component"));
	//WidgetInteractionComp->SetupAttachment(WidgetInteractionComp);

	if (WidgetInteractionComp)
	{
		WidgetInteractionComp->InteractionDistance = 2000.0f;

		WidgetInteractionComp->bEnableHitTesting = true;

	
	}
}

// Called when the game starts or when spawned
void AMiniGameActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMiniGameActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


