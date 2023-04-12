// Fill out your copyright notice in the Description page of Project Settings.


#include "PJH_VoteActor.h"
#include "Components/WidgetComponent.h"

// Sets default values
APJH_VoteActor::APJH_VoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
}

// Called when the game starts or when spawned
void APJH_VoteActor::BeginPlay()
{
	Super::BeginPlay();
	
	widgetComp->SetVisibility(false);
}

// Called every frame
void APJH_VoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

