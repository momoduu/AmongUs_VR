// Fill out your copyright notice in the Description page of Project Settings.


#include "PJH_CrewFSM.h"
#include "PJH_Crew.h"
#include "AIController.h"
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <NavigationSystem/Public/NavigationSystemTypes.h>
#include <AIModule/Classes/AITypes.h>
#include <NavigationSystem/Public/NavigationData.h>
#include <AI/Navigation/NavigationTypes.h>
#include <Engine/EngineTypes.h>
#include "PJH_GameModeBase.h"
#include <Math/MathFwd.h>
#include "Components/CapsuleComponent.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PJH_CrewAnim.h"
#include "HJ_VR_Player.h"
#include <Materials/Material.h>
#include <Engine/SkeletalMesh.h>
#include "PJH_MovePoint.h"
#include <Sound/SoundBase.h>

// Sets default values for this component's properties
UPJH_CrewFSM::UPJH_CrewFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/PJH/Mesh/NewFolder/dead.dead_Died'"));
	if (tempMesh.Succeeded()) {
		dieMesh = tempMesh.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("/Script/Engine.SoundWave'/Game/SHJ/Sound/sound_Wav/Vote.Vote'"));
	if (tempSound.Succeeded()) {
		voteSound = tempSound.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("/Script/Engine.SoundWave'/Game/SHJ/Sound/sound_Wav/Imposter_Kill.Imposter_Kill'"));
	if (tempSound2.Succeeded()) {
		killSound = tempSound2.Object;
	}
}


// Called when the game starts
void UPJH_CrewFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// me = enemy 지정
	me = Cast<APJH_Crew>(GetOwner());
	// enemy ai 지정
	ai = Cast<AAIController>(me->GetController());

	// 본인 제외 어떻게?
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APJH_Crew::StaticClass(), others);
	AActor* temp = UGameplayStatics::GetActorOfClass(GetWorld(), AHJ_VR_Player::StaticClass());
	others.Add(temp);
	//delete my
	for (int32 i = 0; i < others.Num(); i++) {
		if (others[i]->GetName() == me->GetName()) {
			meIdx = i; break;
		}
	}
	
	// 시간 의심도 추가
	for (int32 i = 0; i < others.Num(); i++) {
		suspicion.Add(0);
		timeFlow.Add(5.0f);
	}

	startPos = me->GetActorLocation();

	anim = Cast<UPJH_CrewAnim>(me->GetMesh()->GetAnimInstance());
	me->GetMesh()->SetMaterial(1, mat);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APJH_MovePoint::StaticClass(), movePoints);
	getRandomPos();
}


// Called every frame
void UPJH_CrewFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	switch (mState) {
	case ECrewState::Idle:
		IdleState();
		break;
	case ECrewState::Move:
		MoveState();
		break;
	case ECrewState::Die:
		DieState();
		break;
	case ECrewState::Vote:
		VoteState();
		break;
	case ECrewState::Export:
		ExportState();
		break;
	}
	updateSuspicion();
	// 나중에 500을 변수로
	//DrawDebugSphere(GetWorld(), me->GetActorLocation(), 500, 20, FColor::Cyan, false);
}

void UPJH_CrewFSM::IdleState() {
	if (isWaitComplete(3.0f)) {
		changeState(ECrewState::Move);
	}
	foundBody();
}
void UPJH_CrewFSM::MoveState() {
	foundBody();

	//navigation setting
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(3);
	req.SetGoalLocation(destination);
	ai->BuildPathfindingQuery(req, query);
	FPathFindingResult r = ns->FindPathSync(query);

	if ((r.Result == ENavigationQueryResult::Success)) { // navigation path found
		auto result = ai->MoveToLocation(destination);

		if (result == EPathFollowingRequestResult::AlreadyAtGoal) { // reached destination
			getRandomPos();
			// 미션 게이지 상승
			changeState(ECrewState::Idle);
			APJH_GameModeBase* gamemode = Cast<APJH_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			if (!gamemode->bMissionGauge) {
				gamemode->missionGauge += 10;
			}
		}
	}
}
void UPJH_CrewFSM::DieState() {
	ai->StopMovement();
}
void UPJH_CrewFSM::VoteState() {
	ai->StopMovement();

	if (currentTime > delayTime && bSend) {
		sendVote();
		currentTime = 0;
		bSend = false;
	}
	else {
		currentTime += GetWorld()->GetDeltaSeconds();
	}
}
void UPJH_CrewFSM::ExportState() {
	//me->SetActorLocation(FVector(-1200, 140, 90));
	me->AddActorWorldRotation(FRotator(0.0f, 1.0f, 1.0f)); // 되긴하네 ㅋㅋㅋㅋ

	if (me->GetActorLocation().X > 4000.0f || me->GetActorLocation().X < -4000.0f) {
		move *= -1;
	}
	me->SetActorLocation(me->GetActorLocation() + FVector(move, 0, 0));
}
void UPJH_CrewFSM::changeState(ECrewState state) {
	mState = state;
	currentTime = 0;
	anim->animState = mState;

	APJH_GameModeBase* gamemode = Cast<APJH_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	switch (mState) {
	case ECrewState::Idle:
		getRandomPos();
		break;
	case ECrewState::Vote:
		ai->StopMovement();
		me->SetActorLocation(startPos);
		delayTime = FMath::RandRange(0.0f, 10.0f);
		break;
	case ECrewState::Die:
		//me->GetMesh()
		me->GetMesh()->SetAnimInstanceClass(nullptr);
		anim = nullptr;

		me->GetMesh()->SetSkeletalMesh(dieMesh);
		me->GetMesh()->SetMaterial(0, mat);
		me->GetMesh()->SetMaterial(1, nullptr);
		me->GetCharacterMovement()->GravityScale = 0.0f;

		
		gamemode->crewLeft -= 1;
		break;
	case ECrewState::Export:
		me->SetActorLocation(FVector(-1200, 5000, 90));
		me->GetCapsuleComponent()->SetEnableGravity(false);
		me->GetMesh()->SetEnableGravity(false);
		me->GetCharacterMovement()->GravityScale = 0.0f;

		gamemode->crewLeft -= 1;
		break;
	}
}
bool UPJH_CrewFSM::isWaitComplete(float delayT) {
	currentTime += GetWorld()->GetDeltaSeconds();

	if (currentTime > delayT) {
		currentTime = 0;
		return true;
	}
	else {
		return false;
	}
}
bool UPJH_CrewFSM::foundBody() { // 탐색
	// overlap 원형 탐색
	FVector center = me->GetActorLocation();
	TArray<FOverlapResult> hitInfos;
	FCollisionObjectQueryParams objectQuerry;
	objectQuerry.AddObjectTypesToQuery(ECC_Pawn);
	FCollisionQueryParams params;
	params.AddIgnoredActor(me);
	
	if (GetWorld()->OverlapMultiByObjectType(hitInfos, center, FQuat::Identity, objectQuerry, FCollisionShape::MakeSphere(500), params)) {
		// 부딪힌 모든 대상 순회
		for (FOverlapResult& hitInfo : hitInfos) {
			
			// 검사 대상 중 Crew 클래스인 경우일때
			APJH_Crew* crew = Cast<APJH_Crew>(hitInfo.GetActor());
			//UE_LOG(LogTemp, Warning, TEXT("%s"), *crew->GetName());
			if (IsValid(crew)) {
				UPJH_CrewFSM* fsm = crew->GetFSM();
				// crew 가 죽은 상태라면
				if (crew->GetFSM()->mState == ECrewState::Die) {
					// 신고

					UE_LOG(LogTemp, Warning, TEXT("Found"));
					Cast<APJH_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->beginVote();
					return true;
				}
				// crew 가 죽지 않은 상태면
				else {
					//UE_LOG(LogTemp, Warning, TEXT("%d"), crew->fsm->mState);
					// 의심도 다운?
					int32 i = GetIndex(crew);
					timeFlow[i] = 0;
				}
			}
			else {
				/*AHJ_VR_Player* player = Cast<AHJ_VR_Player>(hitInfo.GetActor());
				int32 i = GetIndex(player);*/
				timeFlow[5] = 0;
			}
		}
	}

	return false;
}
void UPJH_CrewFSM::updateSuspicion() {
	// 시간은 tick 마다 더해지고 의심도는 안보였을때 1씩 증가, 봤으면 timeflow 0으로
	for (int32 i = 0; i < others.Num(); i++) {
		if (timeFlow[i] > 5.0f) {
			suspicion[i] += 1;
			timeFlow[i] = 0;
		}
		else {
			timeFlow[i] += GetWorld()->GetDeltaSeconds();
		}
	}
}
void UPJH_CrewFSM::getRandomPos() {
	/*auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	ns->GetRandomReachablePointInRadius(me->GetActorLocation(), 500, loc);*/

	AActor* loc = movePoints[FMath::RandRange(0, movePoints.Num() - 1)];
	destination = loc->GetActorLocation();

	
}
int32 UPJH_CrewFSM::GetIndex(AActor* crew) {
	for (int32 i = 0; i < others.Num(); i++) {
		if (others[i]->GetName() == crew->GetName()) {
			return i;
		}
	}
	return -1;
}
void UPJH_CrewFSM::getKilled() {
	// 매시도 바꾸기
	//me->GetMesh()->SetWorldRotation(FQuat(90, 0, 0, 1));
	//me->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), killSound, me->GetActorLocation());
	changeState(ECrewState::Die);
}
void UPJH_CrewFSM::sendVote() {
	if (mState == ECrewState::Export || mState == ECrewState::Die) return;
	// get gamemode
	APJH_GameModeBase* myGameMode = Cast<APJH_GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	// pick one
	int32 idx = getHighest();

	if (idx < 0) { 
		int32 tmp = FMath::RandRange(0, others.Num() - 1);
		APJH_Crew* t = Cast<APJH_Crew>(others[tmp]);
		if (t->fsm->mState != ECrewState::Die && t->fsm->mState != ECrewState::Export) {
			myGameMode->receiveVote(others[tmp]->GetName());
		}
		else {
			myGameMode->receiveSkip();
		}
	}
	else {
		myGameMode->receiveVote(others[idx]->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"), *me->GetName(), *others[idx]->GetName());
	}
	
	UGameplayStatics::PlaySound2D(GetWorld(), voteSound); 
}

int32 UPJH_CrewFSM::getHighest() {
	int32 temp = 0;
	int32 index = 0;
	int32 cnt = 0;

	for (int32 i = 0; i < suspicion.Num(); i++) {
		if (i == meIdx) {}
		else if (others[i]->GetName().Contains(TEXT("PJHCrew"))) {
			APJH_Crew* tmp = Cast<APJH_Crew>(others[i]);
			if (tmp->fsm->mState != ECrewState::Die && tmp->fsm->mState != ECrewState::Export) {
				if (temp < suspicion[i]) {
					temp = suspicion[i];
					index = i;
					cnt++;
				}
			}
		}
		else { // check later
			AHJ_VR_Player* tmp = Cast<AHJ_VR_Player>(others[i]);
			if (temp < suspicion[i]) {
				temp = suspicion[i];
				index = i;
				cnt++;
			}
		}
	}

	if (cnt > 0)
		return index;
	else
		return -1;
}