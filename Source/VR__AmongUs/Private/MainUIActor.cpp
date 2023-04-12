// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include <UMG/Public/Components/WidgetInteractionComponent.h>
#include "HJ_InformationWidget.h"
#include "Components/WidgetComponent.h"
#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AMainUIActor::AMainUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	screenComp->SetupAttachment(RootComponent);

	mainUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	mainUI->SetupAttachment(RootComponent);

// 	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("UIInteraction Component"));
// 	WidgetInteractionComp->SetupAttachment(WidgetInteractionComp);
// 
// 	if (WidgetInteractionComp)
// 	{
// 		WidgetInteractionComp->InteractionDistance = 2000.0f;
// 
// 		WidgetInteractionComp->bEnableHitTesting = true;
// 
// 
// 	}

}

// Called when the game starts or when spawned
void AMainUIActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMainUIActor::Online_Play()
{
	UE_LOG(LogTemp, Log, TEXT("PLAY !!!!!!!!!!"));
	
}
void AMainUIActor::Howto_Play()
{

}
void AMainUIActor::Exit_Play()
{
	UE_LOG(LogTemp, Log, TEXT("Exit !!!!!!!!!!"));
}
