// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PJH_CrewFSM.h"
#include "PJH_CrewAnim.generated.h"

/**
 * 
 */
UCLASS()
class VR__AMONGUS_API UPJH_CrewAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECrewState animState;
};
