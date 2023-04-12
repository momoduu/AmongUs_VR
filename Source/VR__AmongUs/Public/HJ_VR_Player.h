// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HJ_VR_Player.generated.h"

UCLASS()
class VR__AMONGUS_API AHJ_VR_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHJ_VR_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UCameraComponent* cam;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UStaticMeshComponent* gazePointer;

	//Left Component
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMotionControllerComponent* leftController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UTextRenderComponent* leftLog;

	//Right Component
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UMotionControllerComponent* rightController;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UTextRenderComponent* rightLog;
// 
// 	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
// 		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	//Input Mapping
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputMappingContext* myMapping;

	//left/right InputArray
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VR_Settings|Components")
		class UPlayerMoveComponent* moveComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UPlayerGraspComponent* graspComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UWidgetInteractionComponent* widgetPointer_right;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UWidgetPointerComponent* widgetPointerComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UGazeComponent* gazeComp;
public:
	UFUNCTION(BlueprintCallable)
		void Teleport();

	UFUNCTION(BlueprintCallable)
	void Click_X();

	FVector startPos;

	//플레이어가 AI 공격
	UFUNCTION(BlueprintCallable)
		void PlayerAttack();

	UFUNCTION(BlueprintCallable)
		void CallDie();

	//타겟 여부 확인
	UPROPERTY(EditAnywhere)
		bool targetLock = false;
	// 		//AI 캐싱
	// 		UPROPERTY(EditAnywhere)
	// 		class

	void OnTriggerLeft(const struct FInputActionValue& value);

	void Recenter();
	
};
