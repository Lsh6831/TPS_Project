// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"

#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

// 매 프레임 자동 호출 - Tick 직접 구성만 해도 UE 자체가 호출
void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	// 이 Animation Instance를 소유한 폰가져오기
	APawn* pawn = TryGetPawnOwner();
	if (pawn==nullptr) return;
	
	//ATPSPlayer 폰의 속도 측정
	ATPSPlayer* player=Cast<ATPSPlayer>(pawn);
	if (player)
	{
		//스피드 확인
		speed=player->GetVelocity().Size();
		
		// 공중에 떠 있는 상태인지
		UCharacterMovementComponent* movement = player->GetCharacterMovement();
		if (movement)
		{
			isInAir = movement->IsFalling();
		}
	}
	// [DEBUG]
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,0.f,FColor::Green,
			FString::Printf(TEXT("Anim"),speed));
	}
}
