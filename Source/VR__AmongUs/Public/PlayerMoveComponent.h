// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR__AMONGUS_API UPlayerMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* leftThumbstick;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* leftTrigger;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* rightThumbstick;

	UPROPERTY(EditAnywhere)
		class UInputAction* leftThumbstickPress;

	UPROPERTY(EditAnywhere, Category = Inputs)
		float power = 1000;

	UPROPERTY(EditAnywhere, Category = Effect)
		TSubclassOf<class ATeleportRingActor> teleport_fx;


// 	UPROPERTY(EditAnywhere)
// 	float H = 0;
// 	UPROPERTY(EditAnywhere)
// 	float V = 0;

	UFUNCTION(BlueprintCallable)
	void TeleportFade();

private:
	//class AHJ_Player* player;
	class AHJ_VR_Player* player;
	TArray<FVector> lineLoc;
	bool bIsShowLine = false;
	class UWorld* currentWorld;
	class ATeleportRingActor* spawned_fx;

	void RotateAxis(const struct FInputActionValue& value);
	void Move(const struct FInputActionValue& value);
	void DrawMoveLine();
	void Teleport();
	void ShowLine();
	void HideLine();
	void InputJump();
	

	//키보드 이동
	void InputHorizontal(float value);
	void InputVertical(float value);
};
