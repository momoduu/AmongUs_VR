// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPointerComponent.h"
#include "EnhancedInputComponent.h"
#include "HJ_Player.h"
#include "Components/WidgetInteractionComponent.h"
#include "HJ_VR_Player.h"
#include "MiniGameActor.h"
#include "HJ_InformationWidget.h"
// Sets default values for this component's properties
UWidgetPointerComponent::UWidgetPointerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	miniGame = CreateDefaultSubobject<AMiniGameActor>(TEXT("Mini Game"));
	infoWidget = CreateDefaultSubobject<UHJ_InformationWidget>(TEXT("Information Widget"));
}


// Called when the game starts
void UWidgetPointerComponent::BeginPlay()
{
	Super::BeginPlay();
	//Player = Cast<AHJ_Player>(GetOwner());
	player = Cast<AHJ_VR_Player>(GetOwner());
	
	
}


// Called every frame
void UWidgetPointerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWidgetPointerComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(right_Trigger, ETriggerEvent::Started, this,
		&UWidgetPointerComponent::ClickWidget);
 	PlayerInputComponent->BindAction(right_Trigger, ETriggerEvent::Completed, this,
 		&UWidgetPointerComponent::ReleaseWidget);
}

void UWidgetPointerComponent::ClickWidget()
{
	//DrawDebugLine(GetWorld(), player->rightHand->GetComponentLocation(), -(player->rightHand->GetForwardVector()) * 500, FColor::Red,false,2,0,2);

	player->widgetPointer_right->PressPointerKey(EKeys::LeftMouseButton);
	UE_LOG(LogTemp,Warning,TEXT("Right Trigger "));
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString(TEXT("Right Trigger ")));
}

void UWidgetPointerComponent::ReleaseWidget()
{
	player->widgetPointer_right->ReleasePointerKey(EKeys::LeftMouseButton);
}
