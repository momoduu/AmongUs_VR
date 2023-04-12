// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerGraspComponent.generated.h"

UENUM()
enum class EGrabType : uint8
{

	ELine UMETA(DisplayName = "LineType"),
	ESweep UMETA(DisplayName = "SweepType"),
	EOverlap UMETA(DisplayName = "OverlapType")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR__AMONGUS_API UPlayerGraspComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerGraspComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* grip_right;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* btn_A;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* trigger_right;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* trigger_right_touch;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumb_right;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumb_right_touch;

	UPROPERTY(EditAnywhere, Category = Inputs)
		float grabDistance = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
		EGrabType myGrabType = EGrabType::ELine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
		float throwPower = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
		float torquePower = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
		class UHapticFeedbackEffect_Base* grabHaptic;


private:
	//class AHJ_Player* player;
	class AHJ_VR_Player* player;
	class UHJ_VR_HandAnimInstance* rightHandAnim;
	bool  bIsGrab = false;
	class APickUpActor* grabedObject;
	bool physicsState = false;
	FVector prevLocation;
	FVector prevForward;
	FVector throwDirection;
	bool bIsReaby = false;

	void GripRightAction(const struct FInputActionValue& value);
	void GripRightRelease(const struct FInputActionValue& value);
	void TriggerRightAction(const struct FInputActionValue& value);
	void TrigerRightTouch();
	void TrigerRightTouchEnd();
	void ThumbRightTouch();
	void ThumbRightTouchEnd();
	void ResetRightFingers();
	void GrabObject(USkeletalMeshComponent* selectHand);
	void ReleaseObject(USkeletalMeshComponent* selectHand, FVector torque);
	void DrawGrabRange();
	void ReadyToShoot();
};
