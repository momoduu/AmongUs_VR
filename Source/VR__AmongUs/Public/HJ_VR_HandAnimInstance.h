// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HJ_VR_HandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VR__AMONGUS_API UHJ_VR_HandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animations)
		float PoseAlphaGrasp = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animations)
		float PoseAlphaIndexCurl = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animations)
		float PoseAlphaPoint = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animations)
		float PoseAlphaThumbUp = 0;
};
