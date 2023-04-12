// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetPointerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR__AMONGUS_API UWidgetPointerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetPointerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere)
		class UInputAction* right_Trigger;
	UPROPERTY(EditAnywhere)
	class AMiniGameActor* miniGame;
	UPROPERTY(EditAnywhere)
	class UHJ_InformationWidget* infoWidget;
	
private:

	//class AHJ_Player* player;
	class AHJ_VR_Player* player;

	

	void ClickWidget();
	void ReleaseWidget();

};
