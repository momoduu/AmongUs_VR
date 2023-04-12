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

	// �÷��̾� ĳ���Ϳ� ������ �ִϸ��̼� �ν��Ͻ��� ĳ���Ѵ�.
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
	// ���������� ��ü ���
	GrabObject(player->rightHand);
	//�����տ� ���� ȿ�� �ֱ�
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

// �հ��� �ִϸ��̼� ������ �⺻ ������ �ǵ�����
void UPlayerGraspComponent::ResetRightFingers()
{
	rightHandAnim->PoseAlphaGrasp = 0;
	rightHandAnim->PoseAlphaIndexCurl = 0;
	rightHandAnim->PoseAlphaPoint = 1;
	rightHandAnim->PoseAlphaThumbUp = 1;
}

// ��ü�� ��� �Լ�
void UPlayerGraspComponent::GrabObject(USkeletalMeshComponent* selectHand)
{

	if (myGrabType == EGrabType::ELine)
	{

		// ��� 1 - LineTrace ���
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
		//��� 2 - SphereTrace ���
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
			//���� �˻��� ��ü�� Physics �� simulate ���̶��
			if (IsValid(grabedObject))
				//if (grabedObject != nullptr)
			{
				//���⼭ ���� �ȵ�
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
		// ���3 - overlap ������� �˻��ϱ�
		FVector center = selectHand->GetComponentLocation();
		TArray<FOverlapResult> hitInfos;
		FCollisionObjectQueryParams objectQuerry;
		FCollisionQueryParams params;
		objectQuerry.AddObjectTypesToQuery(ECC_WorldDynamic);
		params.AddIgnoredActor(player);

		if (GetWorld()->OverlapMultiByObjectType(hitInfos, center, FQuat::Identity, objectQuerry,
			FCollisionShape::MakeSphere(grabDistance), params))
		{

			//�ε��� ����� ��ȸ�Ѵ�.
			for (FOverlapResult hitInfo : hitInfos)
			{

				//�˻� ��� �߿� PickUP Actor Ŭ������ ��쿡�� �տ� ���δ�
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
	//���� ��� �ִ� ��ü�� �ִٸ� 
	if (grabedObject != nullptr)
	{
		//��� �ִ� ��ü�� �����.
		grabedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		//��ü�� ���� ������  on/off ���θ� �ǵ��� �ش�.
		UBoxComponent* boxComp = Cast<UBoxComponent>(grabedObject->GetRootComponent());
		if (boxComp != nullptr)
		{
			boxComp->SetSimulatePhysics(true);
			boxComp->SetEnableGravity(true);
		}

		throwDirection.Normalize();

		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3 ,%.3f, %.3f"),
			throwDirection.X, throwDirection.Y, throwDirection.Z));

		//���� ������ ����� ���Ѵ�.
		boxComp->AddImpulse(throwDirection * throwPower);

		boxComp->AddTorqueInDegrees(torque * torquePower, NAME_None, true);

		//grabedObject ������ ������  nullptr �� �����Ѵ�.
		grabedObject = nullptr;
	}
}
void UPlayerGraspComponent::DrawGrabRange()
{

	if (myGrabType == EGrabType::ELine)
	{
		// 	//�� �׸���	(Line)
		FVector startLoc = player->rightHand->GetComponentLocation();
		FVector endLoc = startLoc + player->rightHand->GetRightVector() * grabDistance;
		DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Cyan, false, -1, 0, 2);


	}
	else if (myGrabType == EGrabType::ESweep || myGrabType == EGrabType::EOverlap)
	{

		FVector center = player->rightHand->GetComponentLocation();
		//player->rightHand->GetRightVector();

		//�� �׸���(Sphere)

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


			//prevLocation ���� ���� ����ϰ� ����
			prevLocation = player->rightController->GetComponentLocation();
			prevForward = player->rightController->GetForwardVector();

		}
		else
		{

			//���� ����
			throwDirection = player->rightController->GetComponentLocation() - prevLocation;


			//ȸ��
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
