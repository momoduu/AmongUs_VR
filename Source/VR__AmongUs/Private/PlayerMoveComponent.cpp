// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "HJ_Player.h"
#include "Components/CapsuleComponent.h"
#include "TeleportRingActor.h"
#include "NiagaraComponent.h"
#include "HJ_VR_Player.h"

// Sets default values for this component's properties
UPlayerMoveComponent::UPlayerMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	//������ �÷��̾� Ŭ������ ĳ���Ѵ�.
	//player = Cast<AHJ_Player>(GetOwner());
	player = Cast<AHJ_VR_Player>(GetOwner());
	
	

	//���� ���� Ŭ������ ĳ���Ѵ�.
	currentWorld = Cast<UWorld>(GetWorld());
	
}


// Called every frame
void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsShowLine)
	{
		DrawMoveLine();
	}
}

void UPlayerMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{

	PlayerInputComponent->BindAction(leftThumbstick, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::RotateAxis);
	PlayerInputComponent->BindAction(rightThumbstick, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::Move);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UPlayerMoveComponent::ShowLine);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Completed, this, &UPlayerMoveComponent::HideLine);
	PlayerInputComponent->BindAction(leftThumbstickPress, ETriggerEvent::Started, this, &UPlayerMoveComponent::InputJump);

	
	
}
void UPlayerMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D val = value.Get<FVector2D>();
	FVector direction = FVector(val.Y, val.X, 0);
	FVector forward = player->GetActorForwardVector() * direction;

	player->AddMovementInput(forward.GetSafeNormal(), 1, false);

}
void UPlayerMoveComponent::InputJump()
{
	player->Jump();
}

void UPlayerMoveComponent::RotateAxis(const struct FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();

	// axis ���� �̿��ؼ� ĳ����(��Ʈ�ѷ�)�� ȸ���Ѵ�.
	player->AddControllerYawInput(axis.X);
	//player->AddControllerPitchInput(axis.Y * -1.0f);
}

void UPlayerMoveComponent::DrawMoveLine()
{
	//�̵��� ��� ȭ�鿡 �׸���.
	//1. �̵��� ��ġ ������ ���Ѵ�.(TArray<FVector>) 

	//1-1. p = p0 + vt - 0.5 * g * t^2
	float timeInterval = 0.02f;
	int32 timeSegment = 50;
	// 
	// 	FVector dir = (FVector::ForwardVector + FVector::UpVector);
	// 	dir = player->leftHand->GetComponentTransform().TransformVector(dir.GetSafeNormal());

		//�޼� �𵨸��� ���� ��ǥ�� ���缭 forward ����� up ������ �ٽ� ����Ѵ�.
	FVector handForward = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::Y);
	FVector handUp = FRotationMatrix(player->leftHand->GetComponentRotation()).GetUnitAxis(EAxis::X) * -1;

	//�ٽ� ����� �� �������� dir �� ���Ѵ�.
	FVector dir = handForward + handUp;

	lineLoc.Empty();
	for (int32 i = 0; i < timeSegment; i++)
	{
		//�ҿ� �ð�
		float timeTaken = timeInterval * i;
		//p = p0 + vt
		FVector prediction = player->leftHand->GetComponentLocation() + dir * power * timeTaken;

		//p.Z -= O.5 * g * t^2
		prediction.Z += 0.5f * GetWorld()->GetDefaultGravityZ() * timeTaken * timeTaken;

		//1-2. 0.1�ʾ� 0~50 ��° ������ ����ؼ� �迭�� �ִ´�.
		lineLoc.Add(prediction);

		//����Ʈ���̽��� �������� üũ�Ѵ�.

		FHitResult hitInfo;

		if (i > 0)
		{

			if (currentWorld->LineTraceSingleByChannel(hitInfo, lineLoc[i - 1], lineLoc[i], ECC_Visibility) && i > 0)
			{
				//�ε��� ������ ������ ��ǥ�� �ְ� �ݺ����� ���� �����Ѵ�.
				lineLoc.Add(hitInfo.ImpactPoint);
				break;

			}
		}
	}

	//2. �տ��� ���� ��ġ ���� ������ ������ �׸��� �׸���.(DrawLine)
	for (int32 i = 0; i < lineLoc.Num() - 1; i++)
	{
		DrawDebugLine(GetWorld(), lineLoc[i], lineLoc[i + 1], FColor::Green, false, -1, 0, 2);
	}

	// �ڷ���Ʈ �� ����Ʈ�� ������ ���� ��ġ�� ��ġ�Ѵ�.
	if (spawned_fx == nullptr)
	{
		// ����Ʈ�� �����Ѵ�.
		spawned_fx = currentWorld->SpawnActor<ATeleportRingActor>(teleport_fx, lineLoc[lineLoc.Num() - 1], FRotator::ZeroRotator);
	}
	else
	{
		// �Ⱥ��̰� ó���� ����Ʈ�� �ٽ� ���̰� �Ѵ�.
		spawned_fx->niagara_fx->SetVisibility(true);
		spawned_fx->SetActorLocation(lineLoc[lineLoc.Num() - 1]);
	}
}



void UPlayerMoveComponent::Teleport()
{
	//����Ų �������� ���� �̵� (��� ��ǥ ����) �Ѵ�.
// 	FVector targetLoc = lineLoc[lineLoc.Num() - 1];
// 	targetLoc.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
// 	player->SetActorLocation(targetLoc, false, nullptr, ETeleportType::TeleportPhysics);
// 
// 	if (spawned_fx != nullptr)
// 	{
// 		spawned_fx->niagara_fx->SetVisibility(false);
// 	}

}

void UPlayerMoveComponent::ShowLine()
{
	bIsShowLine = true;
}
void UPlayerMoveComponent::HideLine()
{
	bIsShowLine = false;
	//Teleport();
	TeleportFade();
}

void UPlayerMoveComponent::TeleportFade()
{
	// ī�޶� Fade in �Ѵ�.
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraFade(0, 1.0f, 1.0f, FLinearColor::Black);

	FTimerHandle fadeTimer;
	GetWorld()->GetTimerManager().SetTimer(fadeTimer, this, &UPlayerMoveComponent::Teleport, 1.0f, false);
	//GetWorld()->GetTimerManager().SetTimer(fadeTimer, this, [&]() { };, 1.0f, false);
}