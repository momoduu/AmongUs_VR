// Fill out your copyright notice in the Description page of Project Settings.


#include "HJ_InformationWidget.h"
#include "Components/Button.h"
#include "MainUIActor.h"



void UHJ_InformationWidget::NativeConstruct()
{
	Btt_Online->OnPressed.AddDynamic(this, &UHJ_InformationWidget::Online);
	Btt_HowtoPlay->OnPressed.AddDynamic(this, &UHJ_InformationWidget::HowtoPlay);
	Btt_Exit->OnPressed.AddDynamic(this, &UHJ_InformationWidget::Exit);
}

void UHJ_InformationWidget::Online() {
	mainUIActor->Online_Play();
}
void UHJ_InformationWidget::HowtoPlay() {

}
void UHJ_InformationWidget::Exit() {

}