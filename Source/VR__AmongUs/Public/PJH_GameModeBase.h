// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PJH_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VR__AMONGUS_API APJH_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APJH_GameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> crews;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> vote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 skipVote = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> voteWidget;

	UPROPERTY()
	class UUserWidget* voteUI;

	UPROPERTY()
	class UWidgetComponent* voteComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class APJH_VoteActor* voteActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class APJH_VoteActor* exportActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class APJH_VoteActor* tieActor;

	UPROPERTY(EditAnywhere)
	AActor* showResultCam;

	FTimerHandle tHandle;

	bool isSequenceEnd = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool setvisibilty = false;

	UFUNCTION(BlueprintCallable)
	void beginVote();

	void receiveVote(FString crewName);
	void receiveSkip();

	UFUNCTION(BlueprintCallable)
	void endVote();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString exportName;

	void showVoteResult();
	void returnCamera();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool showTieUI = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 missionGauge = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 crewLeft = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMissionGauge = false;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
