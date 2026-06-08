// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPSPlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPS_PROJECT_API UTPSPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	// 캐릭터의 전후 이동 속도(양수 = 전진, 음수 = 후진) - Blend Space의 새로축
	UPROPERTY(BlueprintReadOnly,Category=PlayerAnim)
	float speed = 0.f;
	
	
	// 캐릭터의 좌우 이동 방향 (양수 = 우측,음수 = 좌측) - Blend Space의 가로축
	UPROPERTY(BlueprintReadOnly,Category=PlayerAnim)
	float direction =0.f;
	
	UPROPERTY(BlueprintReadOnly,Category=PlayerAnim)
	bool isInAir = false;
};
