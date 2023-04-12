// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "HJ_Player.generated.h"

UCLASS()
class VR__AMONGUS_API AHJ_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHJ_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Head Component
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UCameraComponent* cam;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UStaticMeshComponent* gazePointer;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UStaticMeshComponent* headMesh;

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

	UPROPERTY(EditAnywhere, Category = "VR_Settings|ModuleSetting")
		TEnumAsByte<enum EHMDTrackingOrigin::Type> trackOrigin;

	//Input Mapping
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		class UInputMappingContext* myMapping;


	//left/right InputArray
	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> leftInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Inputs")
		TArray<class UInputAction*> rightInputs;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UPlayerMoveComponent* moveComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UPlayerGraspComponent* graspComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UWidgetInteractionComponent* widgetPointer_right;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UWidgetPointerComponent* widgetPointerComp;




	//Ű���� �� 
	UPROPERTY(EditAnywhere)
		float H = 0;
	UPROPERTY(EditAnywhere)
		float V = 0;
	UPROPERTY(EditAnywhere)
		float Speed = 600;
	UPROPERTY(EditAnywhere)
		float mx = 0;
	UPROPERTY(EditAnywhere)
		float my = 0;
	//���콺 ȸ�� 
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* tpsCamComp;

	UPROPERTY(EditAnywhere, Category = "VR_Settings|Components")
		class UGazeComponent* gazeComp;

public:
	UFUNCTION(BlueprintCallable)
	void Teleport();


	//�÷��̾ AI ����
	UFUNCTION(BlueprintCallable)
		void PlayerAttack();
	//Ÿ�� ���� Ȯ��
	UPROPERTY(EditAnywhere)
		bool targetLock = false;
	// 		//AI ĳ��
	// 		UPROPERTY(EditAnywhere)
	// 		class



	void OnTriggerLeft(const struct FInputActionValue& value);

	void Recenter();

	//Ű���� �̵�
	void InputHorizontal(float value);
	void InputVertical(float value);
	void Turn(float value);
	void LookUp(float value);

};
