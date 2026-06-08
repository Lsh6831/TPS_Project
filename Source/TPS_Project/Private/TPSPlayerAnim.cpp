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
		
		// 속도 , 전후 좌우 백터가져오기
		FVector velocity = player->GetVelocity();
		FVector forward = player->GetActorForwardVector();
		FVector right = player->GetActorRightVector();
		
		//DotProduct() 로 전후 성분과 좌우 성분을 분리 -> Bledn space 의 8방향 분리
		speed = FVector::DotProduct(velocity,forward); // 앞=양수, 뒤 = 음수
		direction = FVector::DotProduct(velocity,right); // 우=양수, 좌 = 은수
		
		
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
			FString::Printf(TEXT("Speed : %.1f direction = %.1f isInAir=%s"),
				speed,direction,isInAir? TEXT("true") : TEXT("false")));
	}
}
