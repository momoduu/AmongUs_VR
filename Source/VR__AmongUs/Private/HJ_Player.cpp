// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_Player.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
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

// Sets default values
AHJ_Player::AHJ_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//키보드 마우스 회전

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 100;

	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	springArmComp->bUsePawnControlRotation = true;
 	tpsCamComp->bUsePawnControlRotation = true;
 	bUseControllerRotationYaw = true;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cam->SetupAttachment(RootComponent);

	gazePointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gaze Pointer"));
	gazePointer->SetupAttachment(cam);
	gazePointer->SetRelativeLocation(FVector(300, 0, 0));

	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	headMesh->SetupAttachment(cam);
	headMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	leftController->SetupAttachment(RootComponent);
	// 모션 소스 선택
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

	//액터 컴포넌트 추가
	moveComp = CreateDefaultSubobject<UPlayerMoveComponent>(TEXT("Move Component"));
	graspComp = CreateDefaultSubobject<UPlayerGraspComponent>(TEXT("Grasp Component"));
	widgetPointerComp = CreateDefaultSubobject<UWidgetPointerComponent>(TEXT("Widget Pointer Component"));
	gazeComp = CreateDefaultSubobject<UGazeComponent>(TEXT("Gaze Component"));
}

// Called when the game starts or when spawned
void AHJ_Player::BeginPlay()
{
	Super::BeginPlay();
	// 헤드 장비의 기준 위치를 설정한다.
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(trackOrigin.GetValue());

	//1. 플레이어 컨트롤러를 가져온다.
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();

	//2. 플레이어 컨트롤러에서 EnhancedInputLocalPlayerSubSystem 를 가져온다.
	UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerCon->GetLocalPlayer());
	//3. 가져온 SubSystemdp IMC 를 등록한다. (우선 순위 0번)
	subsys->AddMappingContext(myMapping, 0);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	//tpsCamComp->bUsePawnControlRotation = true;
	//tpsCamComp->bUsePawnControlRotation = false;

	APlayerController* playerContoller = Cast<class APlayerController>(Controller);
	playerContoller->PlayerCameraManager->ViewPitchMin = -45;
	playerContoller->PlayerCameraManager->ViewPitchMax = 45;
	
}

// Called every frame
void AHJ_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//player 키보드 이동
	FVector p0 = GetActorLocation();
	FVector vt = GetActorForwardVector() * V + GetActorRightVector() * H;
	vt.GetSafeNormal();
	FVector p = p0 + vt * Speed * DeltaTime;
	SetActorLocation(p);

	//player 키보드 회전
	SetActorRotation(FRotator(0, mx, 0));
	SetActorRotation(FRotator(-my, 0, 0));

}

// Called to bind functionality to input
void AHJ_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//키보드 이동
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AHJ_Player::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AHJ_Player::InputVertical);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AHJ_Player::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AHJ_Player::LookUp);
	//vr input 아직 안만듬
	
	PlayerInputComponent->BindAction(TEXT("PlayerAttack"), IE_Released, this, &AHJ_Player::PlayerAttack);


	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Triggered, this, &AHJ_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[0], ETriggerEvent::Completed, this, &AHJ_Player::OnTriggerLeft);
		enhancedInputComponent->BindAction(leftInputs[1], ETriggerEvent::Started, this, &AHJ_Player::Recenter);
		enhancedInputComponent->BindAction(leftInputs[2], ETriggerEvent::Started, this, &AHJ_Player::PlayerAttack);

		moveComp->SetupPlayerInputComponent(enhancedInputComponent);
		graspComp->SetupPlayerInputComponent(enhancedInputComponent);
		widgetPointerComp->SetupPlayerInputComponent(enhancedInputComponent);
	}
}

void AHJ_Player::OnTriggerLeft(const struct FInputActionValue& value)
{
	float  val = value.Get<float>();

	//왼손 로그에 값을 
	FString msg = FString::Printf(TEXT("%f"), val);
	leftLog->SetText(FText::FromString(msg));
}

void AHJ_Player::Recenter()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AHJ_Player::InputHorizontal(float value)
{
	H = value;
}
void AHJ_Player::InputVertical(float value)
{
	V = value;
}

void AHJ_Player::Turn(float value)
{
	//mx += value;
	AddControllerYawInput(value);
	
}
void AHJ_Player::LookUp(float value)
{
	AddControllerPitchInput(value);
	//my = FMath::Clamp(my, -45, 45);
}



void AHJ_Player::Teleport()
{
// 	
// 	FVector targetLoc ;
// 	targetLoc.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
// 	SetActorLocation(targetLoc, false, nullptr, ETeleportType::None);
}

void AHJ_Player::PlayerAttack()
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
		// 부딪힌 모든 대상을 순회한다
		for (FOverlapResult& hitInfo : hitInfos) {
			// 검사 대상 중에 PickUpActor 클래스인 경우에만 손에 붙인다
			APJH_Crew* picks = Cast<APJH_Crew>(hitInfo.GetActor());
			if (picks != nullptr && picks->fsm->mState != ECrewState::Die) {
				picks->fsm->getKilled(); break;
				//UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
			}
		}
	}
}