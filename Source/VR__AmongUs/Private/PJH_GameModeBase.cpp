// Fill out your copyright notice in the Description page of Project Settings.


#include "PJH_GameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "../Public/PJH_Crew.h"
#include "../Public/PJH_CrewFSM.h"
#include "Blueprint/UserWidget.h"
#include <Components/CapsuleComponent.h>
#include <UMG/Public/Blueprint/WidgetBlueprintLibrary.h>
#include "HJ_Player.h"
#include <Camera/CameraActor.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "HJ_VR_Player.h"
#include <UMG/Public/Components/WidgetComponent.h>
#include "PJH_VoteActor.h"
//#include "GameFramework/Actor.h"

APJH_GameModeBase::APJH_GameModeBase() {
	/*ConstructorHelpers::FClassFinder<UUserWidget> tempUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PJH/Blueprints/BP_PJH_Vote2.BP_PJH_Vote2_C'"));
	if (tempUI.Succeeded()) {
		voteWidget = tempUI.Class;
		voteUI = CreateWidget<UUserWidget>(GetWorld(), voteWidget);
	}*/

}

void APJH_GameModeBase::BeginPlay() {
	Super::BeginPlay();

	showResultCam = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());

	voteUI = CreateWidget<UUserWidget>(GetWorld(), voteWidget);

	// ��� ũ��� ���� ������ + ���߿� �÷��̾� �߰�
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APJH_Crew::StaticClass(), crews);
	for (int32 i = 0; i < crews.Num(); i++) {
		vote.Add(0);
	}

	// �÷��̾� ���� ����
	AActor* temp = UGameplayStatics::GetActorOfClass(GetWorld(), AHJ_VR_Player::StaticClass());
	crews.Add(temp);
	vote.Add(0);
	
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APJH_VoteActor::StaticClass(), actors);

	for (AActor* actor : actors) {
		if (actor->GetName().Contains(TEXT("Vote"))) {
			voteActor = Cast<APJH_VoteActor>(actor);
		}
		else if (actor->GetName().Contains(TEXT("Export"))) {
			exportActor = Cast<APJH_VoteActor>(actor);
		}
		else if (actor->GetName().Contains(TEXT("Tie"))) {
			tieActor = Cast<APJH_VoteActor>(actor);
		} // blueprintcallable
	}

}
void APJH_GameModeBase::Tick(float DeltaTime) {
	if (missionGauge >= 100) {
		//UE_LOG(LogTemp, Warning, TEXT("Crews win!"));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GameOverMap"));
	}
	if (crewLeft == 0) {
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("VictoryMap"));
	}
}

void APJH_GameModeBase::beginVote() {
	//voteUI->SetVisibility(ESlateVisibility::Visible);
	//voteUI->AddToViewport();
	voteActor->widgetComp->SetVisibility(true);
	exportActor->widgetComp->SetVisibility(false);
	tieActor->widgetComp->SetVisibility(false);

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetWorld()->GetFirstPlayerController(), voteUI);
	//UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());

	// �Ű��� ǥ��

	for (int32 i = 0; i < crews.Num(); i++) { // �̸����� �з�
		if (crews[i]->GetName().Contains(TEXT("PJHCrew"))) {
			APJH_Crew* c = Cast<APJH_Crew>(crews[i]);
			if (c->fsm->mState != ECrewState::Die && c->fsm->mState != ECrewState::Export){ // ���� �ʾ�����
				c->fsm->changeState(ECrewState::Vote);
			}
		}
		else if (crews[i]->GetName().Contains(TEXT("Player"))) {
			// �÷��̾� ���� �������� �̵�
			AHJ_VR_Player* p = Cast<AHJ_VR_Player>(crews[i]);
			p->SetActorLocation(p->startPos);
		}

	}
}
void APJH_GameModeBase::receiveVote(FString crewName) {
	for (int32 i = 0; i < crews.Num(); i++) {
		if (crews[i]->GetName() == crewName) {
			vote[i] += 1;
		}
	}
}
void APJH_GameModeBase::receiveSkip() {
	skipVote++;
}
void APJH_GameModeBase::endVote() {
	showVoteResult();
	
	// vote ��� �ʱ�ȭ
	skipVote = 0;

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetWorld()->GetFirstPlayerController());

	voteActor->widgetComp->SetVisibility(false);
}
void APJH_GameModeBase::returnCamera() {
	AActor* c = UGameplayStatics::GetActorOfClass(GetWorld(), AHJ_VR_Player::StaticClass());

	//c->cam->GetCam
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(c);

	tieActor->widgetComp->SetVisibility(false);
	exportActor->widgetComp->SetVisibility(false);


	// ī�޶� ���� �� ������
	for (int32 i = 0; i < crews.Num(); i++) {
		if (crews[i]->GetName().Contains(TEXT("PJHCrew"))) {
			APJH_Crew* temp = Cast<APJH_Crew>(crews[i]);
			if (temp->fsm->mState != ECrewState::Die && temp->fsm->mState != ECrewState::Export) {
				vote[i] = 0;
				temp->fsm->mState = ECrewState::Idle;
				temp->fsm->bSend = true;
			}
			else if (temp->fsm->mState == ECrewState::Export) {
				vote[i] = -1;
				temp->fsm->bSend = true;
			}
			else {
				temp->GetMesh()->SetVisibility(false);
				temp->SetActorLocation(FVector(0, 0, 5000));
				temp->GetCharacterMovement()->GravityScale = 0.0f;
				vote[i] = -1;
			}
		}
		else if (crews[i]->GetName().Contains(TEXT("Player"))) {
			// �÷��̾� ��ǥ ��� ����
			vote[i] = 0;
		}

	}
}
void APJH_GameModeBase::showVoteResult() {
	int32 highIdx = 0;
	int32 tmp = vote[0];
	int32 cnt = 0;

	// ���� �� �����ϰ� ���� ���� ��ǥ�� ���� �� ����
	for (int32 i = 0; i < crews.Num(); i++) {
		if (tmp == vote[i]) {
			cnt++;
		}
		else if (tmp < vote[i]) {
			tmp = vote[i];
			highIdx = i;
			cnt = 0;
		}
	}
	// �÷��̾��??
	if (skipVote >= vote[highIdx]) {
		showTieUI = true;
	}
	else if (cnt == 0) {
		if (crews[highIdx]->GetName().Contains(TEXT("PJHCrew"))) {
			APJH_Crew* c = Cast<APJH_Crew>(crews[highIdx]);
			c->fsm->changeState(ECrewState::Export);
			exportName = UKismetSystemLibrary::GetDisplayName(c);
		}
		else { // �÷��̾� ����
			AHJ_VR_Player* c = Cast<AHJ_VR_Player>(crews[highIdx]);
			exportName = TEXT("HJ_VR_Player");
		}
		
		showTieUI = false;
	}
	else {
		showTieUI = true;
	}

	// �Լ��� ���� ������
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(showResultCam);
	if (showTieUI) {
		tieActor->widgetComp->SetVisibility(true);
	}
	else {
		
		exportActor->widgetComp->SetVisibility(true);
	}


	GetWorldTimerManager().SetTimer(tHandle, this, &APJH_GameModeBase::returnCamera, 5.0f, false);
}