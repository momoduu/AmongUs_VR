// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PJH_CrewFSM.generated.h"

UENUM(BlueprintType)
enum class ECrewState : uint8
{
	Idle,
	Move,
	Die,
	Vote,
	Export,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR__AMONGUS_API UPJH_CrewFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPJH_CrewFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FSM)
	ECrewState mState = ECrewState::Idle;

	void IdleState();
	void MoveState();
	void DieState();
	void VoteState();
	void ExportState();

	void changeState(ECrewState state);
	bool isWaitComplete(float delayTime);
	bool foundBody();
	void updateSuspicion();
	void getRandomPos();
	void getKilled();
	void sendVote();
	int32 getHighest();

	int32 GetIndex(AActor* crew);

	int32 meIdx;

	UPROPERTY()
	class APJH_Crew* me;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> others;

	//UPROPERTY(EditAnywhere)
	//TTuple<AActor*, int32> TPair;

	UPROPERTY(EditAnywhere)
	TArray<int32> suspicion;
	TArray<float> timeFlow;

	//UPROPERTY(EditAnywhere)
	//TArray<FTimerHandle> time;

	UPROPERTY(EditAnywhere)
	FVector startPos;

	FVector destination = FVector(0);

	UPROPERTY()
	class AAIController* ai;
	
	UPROPERTY()
	class UPJH_CrewAnim* anim;

	FTimerHandle h1;
	FTimerHandle h2;
	FTimerHandle h3;

	float currentTime;
	float move = 1.0f;
	float delayTime = 0.0f;
	float curTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSend = true;

	class USkeletalMesh* dieMesh;

	UPROPERTY(EditAnywhere)
	class UMaterial* mat;

	UPROPERTY(EditAnywhere, Category = FSM)
	TArray<AActor*> movePoints;

	class USoundBase* voteSound;
	class USoundBase* killSound;
};
