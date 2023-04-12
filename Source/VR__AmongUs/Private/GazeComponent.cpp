// Fill out your copyright notice in the Description page of Project Settings.


#include "GazeComponent.h"
#include "HJ_Player.h"
#include "Camera/CameraComponent.h"
#include "HJ_VR_Player.h"

// Sets default values for this component's properties
UGazeComponent::UGazeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGazeComponent::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AHJ_VR_Player>(GetOwner());
	//player = Cast<AHJ_VR_Player>(GetOwner());
	
	
}


// Called every frame
void UGazeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ChangeSize(min, max);

	FVector startLoc = player->cam->GetComponentLocation();
	FVector endLoc = startLoc + player->cam->GetForwardVector() * 1000.0f;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(player);

	increase = GetWorld()->LineTraceSingleByChannel(hitInfo, startLoc, endLoc, ECC_Visibility, params);
}

void UGazeComponent::ChangeSize(FVector minSize, FVector maxSize)
{

	//만일, increase  라면
		//현재 사이즈에서 maxSize 로 점점 증가시킨다	
	//그렇지 않다면,
		//현재 사이즈에서 minSize로 감소시킨다.
	currentSize = increase ? currentSize + GetWorld()->GetDeltaSeconds() :
		currentSize - GetWorld()->GetDeltaSeconds();

	currentSize = FMath::Clamp(currentSize, 0.0f, 1.0f);
	FVector targetSize = FMath::Lerp(minSize, maxSize, currentSize);

	player->gazePointer->SetRelativeScale3D(targetSize);


}

