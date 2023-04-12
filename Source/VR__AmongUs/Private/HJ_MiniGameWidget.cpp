// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_MiniGameWidget.h"
#include "Components/Button.h"


void UHJ_MiniGameWidget::NativeConstruct()
{
	btn_SkyBlue->OnPressed.AddDynamic(this, &UHJ_MiniGameWidget::TestFunc);
}

void UHJ_MiniGameWidget::TestFunc()
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString(TEXT("Code Blue!!!!!!! ")));
}
