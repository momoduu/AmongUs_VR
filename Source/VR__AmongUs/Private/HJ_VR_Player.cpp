// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_VR_Player.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HJ_VR_IKAnimInstance.h"
#include "Components/TextRenderComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "PlayerMoveComponent.h"
#include "PlayerGraspComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include "Components/WidgetInteractionComponent.h"
#include "WidgetPointerComponent.h"
#include "PJH_Crew.h"
#include "PJH_CrewFSM.h"
#include <HeadMountedDisplayFunctionLibrary.h>
#include "GazeComponent.h"
#include "PJH_GameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AHJ_VR_Player::AHJ_VR_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cam->SetupAttachment(RootComponent);

	//gazePointer 
	gazePointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gaze Pointer"));
	gazePointer->SetupAttachment(cam);
	gazePointer->SetRelativeLocation(FVector(300, 0, 0));

	//left
	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	leftController->SetupAttachment(RootComponent);
	// ��� �ҽ� ����
	leftController->MotionSource = "Left";

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHand"));
	leftHand->SetupAttachment(leftController);
	leftHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	leftHand->SetRelativeRotation(FRotator(-25.0f, 180.0f, 90.0f));


	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log Text"));
	leftLog->SetupAttachment(leftController);
	leftLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	leftLog->SetTextRenderColor(FColor::Yellow);
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextCenter);

	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	rightController->SetupAttachment(RootComponent);
	rightController->MotionSource = "Right";

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHand"));
	rightHand->SetupAttachment(rightController);
	rightHand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	rightHand->SetRelativeRotation(FRotator(25.0f, 0.0f, 90.0f));

	//widget click
	widgetPointer_right = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("Widget Pointer"));
	widgetPointer_right->SetupAttachment(rightHand);

	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log Text"));
	rightLog->SetupAttachment(rightController);
	rightLog->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	rightLog->SetTextRenderColor(FColor::Yellow);
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextCenter);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//���� ������Ʈ �߰�
	moveComp = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("Move Component"));
	graspComp = CreateDefaultSubobject<UPlayerGraspComponent>(TEXT("Grasp Component"));
	widgetPointerComp = CreateDefaultSubobject<UWidgetPointerComponent>(TEXT("Widget Pointer Component"));
	gazeComp = CreateDefaultSubobject<UGazeComponent>(TEXT("Gaze Component"));
}

// Called when the game starts or when spawned
void AHJ_VR_Player::BeginPlay()
{
	Super::BeginPlay();
	// ��� ����� ���� ��ġ�� �����Ѵ�.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	//1. �÷��̾� ��Ʈ�ѷ��� �����´�.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	//2. �÷��̾� ��Ʈ�ѷ����� EnhancedInputLocalPlayerSubSystem �� �����´�.
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());
	//3. ������ SubSystemdp IMC �� ����Ѵ�. (�켱 ���� 0��)
	subsys->AddMappingContext(myMapping, 0);

	startPos = GetActorLocation();
}

// Called every frame
void AHJ_VR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ��� ��Ʈ�ѷ��� ��ġ ���� ȸ�� ���� �ִϸ��̼� �������Ʈ ������ �����Ѵ�.
	UHJ_VR_IKAnimInstance* myAnim = Cast<UHJ_VR_IKAnimInstance>(GetMesh()->GetAnimInstance());

	if (myAnim != nullptr)
	{
		myAnim->controllerLocation_Left = leftController->GetComponentLocation();
		myAnim->controllerLocation_Right = rightController->GetComponentLocation();

		FRotator leftRot = leftController->GetComponentRotation();
		leftRot = FRotator(leftRot.Pitch * -1, leftRot.Yaw * -1, leftRot.Roll);

		FRotator rightRot = rightController->GetComponentRotation();
		rightRot = FRotator(rightRot.Pitch * -1, rightRot.Yaw, rightRot.Roll * -1);

		myAnim->controllerRotation_Left = leftRot;
		myAnim->controllerRotation_Right = rightRot;

		FRotator headRot = cam->GetComponentRotation();
		headRot = FRotator(headRot.Roll, headRot.Pitch * -1, headRot.Yaw * -1);

		myAnim->headLocation = cam->GetComponentLocation();
		myAnim->headRotation = headRot;
	}
}

// Called to bind functionality to input
void AHJ_VR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AHJ_VR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AHJ_VR_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Started, this, &AHJ_VR_Player::Recenter);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &AHJ_VR_Player::PlayerAttack);
		enhancedInputComponent->BindAction(leftInputs[3], ETriggerEvent::Started, this, &AHJ_VR_Player::Click_X);

		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
		widgetPointerComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void AHJ_VR_Player::OnTriggerLeft(const struct FInputActionValue& value)
{
	float  val = value.Get<float>();

	//�޼� �α׿� ���� 
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}

void AHJ_VR_Player::Recenter()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}




void AHJ_VR_Player::Teleport()
{
	// 	
	// 	FVector targetLoc ;
	// 	targetLoc.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	// 	SetActorLocation(targetLoc, false, nullptr, ETeleportType::None);
}

void AHJ_VR_Player::Click_X()
{
	UE_LOG(LogTemp, Warning, TEXT("click_x "));
}

void AHJ_VR_Player::PlayerAttack()
{


	//FHitResult hitInfo;
	//FCollisionQueryParams params;
	//params.AddIgnoredActor(this);
	//FString profileName = FString(TEXT("PickUp"));

	//// sphere trace
	//bool bHit = GetWorld()->SweepSingleByChannel(hitInfo, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(300), params);
	//auto hitComp = hitInfo.GetComponent();

	//DrawDebugSphere(GetWorld(), GetActorLocation(), 300, 24, FColor::Red);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *hitInfo.GetActor()->GetName());
	//APJH_Crew* crew = Cast<APJH_Crew>(hitInfo.GetActor());

	//if (bHit && crew != nullptr) {
	//	UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
	//	
	//	crew->fsm->changeState(ECrewState::Die);
	//}

	TArray<FOverlapResult> hitInfos;
	FCollisionObjectQueryParams objectQuerry;
	objectQuerry.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->OverlapMultiByObjectType(hitInfos, GetActorLocation(), FQuat::Identity, objectQuerry, FCollisionShape::MakeSphere(300), params)) {
		// �ε��� ��� ����� ��ȸ�Ѵ�
		for (FOverlapResult& hitInfo : hitInfos) {
			// �˻� ��� �߿� PickUpActor Ŭ������ ��쿡�� �տ� ���δ�
			APJH_Crew* picks = Cast<APJH_Crew>(hitInfo.GetActor());
			if (picks != nullptr && picks->fsm->mState != ECrewState::Die) {
				picks->fsm->getKilled(); break;
				//UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
			}
		}
	}
}
void AHJ_VR_Player::CallDie()
{
	TArray<FOverlapResult> hitInfos;
	FCollisionObjectQueryParams objectQuerry;
	objectQuerry.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	if (GetWorld()->OverlapMultiByObjectType(hitInfos, GetActorLocation(), FQuat::Identity, objectQuerry, FCollisionShape::MakeSphere(300), params)) {
		// �ε��� ��� ����� ��ȸ�Ѵ�
		for (FOverlapResult& hitInfo : hitInfos) {
			// �˻� ��� �߿� PickUpActor Ŭ������ ��쿡�� �տ� ���δ�
			APJH_Crew* picks = Cast<APJH_Crew>(hitInfo.GetActor());
			if (picks != nullptr && picks->fsm->mState == ECrewState::Die)
			{
				Cast<APJH_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->beginVote();

			}

		}
	}

}
