// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HJ_InformationWidget.generated.h"

/**
 * 
 */
UCLASS()
class VR__AMONGUS_API UHJ_InformationWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* Btt_Online;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Btt_Local;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Btt_HowtoPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Btt_FreePlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UButton* Btt_Exit;

	class AMainUIActor* mainUIActor;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void Online();

	UFUNCTION()
	void HowtoPlay();

	UFUNCTION()
	void Exit();
};
