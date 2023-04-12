// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGraspComponent.h"

#include "HJ_Player.h"
#include "HJ_VR_HandAnimInstance.h"
#include "PickUpActor.h"
#include <Components/TextRenderComponent.h>
#include "Components/SphereComponent.h"
#include <Components/BoxComponent.h>
#include <MotionControllerComponent.h>
#include <Haptics/HapticFeedbackEffect_Base.h>
#include "HJ_VR_Player.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

// Sets default values for this component's properties
UPlayerGraspComponent::UPlayerGraspComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerGraspComponent::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 캐릭터와 오른손 애니메이션 인스턴스를 캐싱한다.
	//player = Cast<AHJ_Player>(GetOwner());
	player = Cast<AHJ_VR_Player>(GetOwner());
	rightHandAnim = Cast<UHJ_VR_HandAnimInstance>(player->rightHand->GetAnimInstance());

	if (rightHandAnim != nullptr)
	{
		ResetRightFingers();
	}

}


// Called every frame
void UPlayerGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}



void UPlayerGraspComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Started, this, &UPlayerGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UPlayerGraspComponent::GripRightRelease);
	PlayerInputComponent->BindAction(btn_A, ETriggerEvent::Started, this, &UPlayerGraspComponent::ReadyToShoot);
	PlayerInputComponent->BindAction(btn_A, ETriggerEvent::Completed, this, &UPlayerGraspComponent::ReadyToShoot);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Started, this, &UPlayerGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Completed, this, &UPlayerGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Started, this, &UPlayerGraspComponent::TrigerRightTouch);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Completed, this, &UPlayerGraspComponent::TrigerRightTouchEnd);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Started, this, &UPlayerGraspComponent::ThumbRightTouch);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Completed, this, &UPlayerGraspComponent::ThumbRightTouchEnd);
}


void UPlayerGraspComponent::GripRightAction(const FInputActionValue& value)
{
	rightHandAnim->PoseAlphaGrasp = value.Get<float>();
	// 오른손으로 물체 잡기
	GrabObject(player->rightHand);
	//오른손에 진동 효과 주기
	GetWorld()->GetFirstPlayerController()->PlayHapticEffect(grabHaptic, EControllerHand::Right, 1, false);
}

void UPlayerGraspComponent::GripRightRelease(const struct FInputActionValue& value)
{
	bIsGrab = false;
	//ReleaseObject(player->rightHand);
}

void UPlayerGraspComponent::TriggerRightAction(const FInputActionValue& value)
{
	rightHandAnim->PoseAlphaIndexCurl = value.Get<float>();
}

void UPlayerGraspComponent::TrigerRightTouch()
{
	rightHandAnim->PoseAlphaPoint = 0;
}

void UPlayerGraspComponent::TrigerRightTouchEnd()
{
	rightHandAnim->PoseAlphaPoint = 1;
}

void UPlayerGraspComponent::ThumbRightTouch()
{
	rightHandAnim->PoseAlphaThumbUp = 0;
}

void UPlayerGraspComponent::ThumbRightTouchEnd()
{
	rightHandAnim->PoseAlphaThumbUp = 1;
}

// 손가락 애니메이션 변수의 기본 값으로 되돌리기
void UPlayerGraspComponent::ResetRightFingers()
{
	rightHandAnim->PoseAlphaGrasp = 0;
	rightHandAnim->PoseAlphaIndexCurl = 0;
	rightHandAnim->PoseAlphaPoint = 1;
	rightHandAnim->PoseAlphaThumbUp = 1;
}

// 물체를 잡는 함수
void UPlayerGraspComponent::GrabObject(USkeletalMeshComponent* selectHand)
{

	if (myGrabType == EGrabType::ELine)
	{

		// 방법 1 - LineTrace 방식
		FVector startLoc = selectHand->GetComponentLocation();
		FVector endLoc = startLoc + selectHand->GetRightVector() * grabDistance;
		FHitResult hitInfo;

		if (GetWorld()->LineTraceSingleByObjectType(hitInfo, startLoc, endLoc, ECC_GameTraceChannel2))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, hitInfo.GetActor()->GetName());
			hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
			grabedObject = Cast<APickUpActor>(hitInfo.GetActor());
			UE_LOG(LogTemp, Error, TEXT("hitInfo = %s"), *grabedObject->GetName());
		}
	}
	else if (myGrabType == EGrabType::ESweep)
	{
		//방법 2 - SphereTrace 방식
		FVector crenter = selectHand->GetComponentLocation();
		//FVector endloc = crenter + selectHand->GetUpVector() * grabDistance;
		FHitResult hitInfo;
		FString profileName = FString(TEXT("PickUp"));
		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		if (GetWorld()->SweepSingleByChannel(hitInfo, crenter, crenter, FQuat::Identity, ECC_Visibility,
			FCollisionShape::MakeSphere(grabDistance), params) && grabedObject == nullptr)
		{
			player->rightLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
			grabedObject = Cast<APickUpActor>(hitInfo.GetActor());
			UE_LOG(LogTemp, Warning, TEXT("ESWEEP start"));
			//만일 검색된 물체의 Physics 가 simulate 중이라면
			if (IsValid(grabedObject))
				//if (grabedObject != nullptr)
			{
				//여기서 부터 안됨
				UE_LOG(LogTemp, Warning, TEXT("ESWEEP OVERLAP!!!!!!!"));

				UBoxComponent* boxComp = Cast<UBoxComponent>(grabedObject->GetRootComponent());
				if (boxComp != nullptr)
				{
					physicsState = boxComp->IsSimulatingPhysics();
					boxComp->SetSimulatePhysics(false);
				}

				hitInfo.GetActor()->AttachToComponent(selectHand,
					FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));


			}

		}

	}
	else if (myGrabType == EGrabType::EOverlap)
	{
		// 방법3 - overlap 방식으로 검사하기
		FVector center = selectHand->GetComponentLocation();
		TArray<FOverlapResult> hitInfos;
		FCollisionObjectQueryParams objectQuerry;
		FCollisionQueryParams params;
		objectQuerry.AddObjectTypesToQuery(ECC_WorldDynamic);
		params.AddIgnoredActor(player);

		if (GetWorld()->OverlapMultiByObjectType(hitInfos, center, FQuat::Identity, objectQuerry,
			FCollisionShape::MakeSphere(grabDistance), params))
		{

			//부딪힌 대상을 순회한다.
			for (FOverlapResult hitInfo : hitInfos)
			{

				//검사 대상 중에 PickUP Actor 클래스인 경우에만 손에 붙인다
				APickUpActor* picks = Cast<APickUpActor>(hitInfo.GetActor());
				if (picks != nullptr)
				{
					hitInfo.GetActor()->AttachToComponent(selectHand,
						FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("GrabPoint"));

				}
			}
		}

	}

	bIsGrab = true;

}

void UPlayerGraspComponent::ReleaseObject(USkeletalMeshComponent* selectHand, FVector torque)
{
	//만일 잡고 있던 물체가 있다면 
	if (grabedObject != nullptr)
	{
		//잡고 있던 물체를 떼어낸다.
		grabedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//물체의 본래 피직스  on/off 여부를 되돌려 준다.
		UBoxComponent* boxComp = Cast<UBoxComponent>(grabedObject->GetRootComponent());
		if (boxComp != nullptr)
		{
			boxComp->SetSimulatePhysics(true);
			boxComp->SetEnableGravity(true);
		}

		throwDirection.Normalize();

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3 ,%.3f, %.3f"),
			throwDirection.X, throwDirection.Y, throwDirection.Z));

		//구한 방향대로 충격을 가한다.
		boxComp->AddImpulse(throwDirection * throwPower);

		boxComp->AddTorqueInDegrees(torque * torquePower, NAME_None, true);

		//grabedObject 포인터 변수를  nullptr 로 변경한다.
		grabedObject = nullptr;
	}
}
void UPlayerGraspComponent::DrawGrabRange()
{

	if (myGrabType == EGrabType::ELine)
	{
		// 	//선 그리기	(Line)
		FVector startLoc = player->rightHand->GetComponentLocation();
		FVector endLoc = startLoc + player->rightHand->GetRightVector() * grabDistance;
		DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Cyan, false, -1, 0, 2);


	}
	else if (myGrabType == EGrabType::ESweep || myGrabType == EGrabType::EOverlap)
	{

		FVector center = player->rightHand->GetComponentLocation();
		//player->rightHand->GetRightVector();

		//선 그리기(Sphere)

		DrawDebugSphere(GetWorld(), center, grabDistance, 30,
			FColor::Cyan, false, -1, 0, 1);

	}
}

void UPlayerGraspComponent::ReadyToShoot()
{
	if (grabedObject != nullptr)
	{
		if (!bIsReaby)
		{
			//DrawGrabRange();


			//prevLocation 갱신 전에 계산하고 저장
			prevLocation = player->rightController->GetComponentLocation();
			prevForward = player->rightController->GetForwardVector();

		}
		else
		{

			//던질 방향
			throwDirection = player->rightController->GetComponentLocation() - prevLocation;


			//회전
			FVector rotAxis = FVector::CrossProduct(prevForward, player->GetActorForwardVector());
			float angle = FMath::Acos(FVector::DotProduct(prevForward, player->rightController->GetForwardVector()));

			angle = FMath::RadiansToDegrees(angle);


			ReleaseObject(player->rightHand, rotAxis * angle);

			DrawDebugLine(GetWorld(), player->rightController->GetComponentLocation(),
				player->rightController->GetComponentLocation() + throwDirection * 50,
				FColor::Red, false, 5, 0, 3);
		}
		bIsReaby = !bIsReaby;
	}
}
