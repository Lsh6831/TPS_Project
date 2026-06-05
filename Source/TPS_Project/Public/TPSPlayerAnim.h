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
	
	// 캐릭터의 현재 이동 속도(ABP State Machine 의 전이 조건에서 사용)
	UPROPERTY(BlueprintReadOnly,Category=PlayerAnim)
	float speed = 0.f;
	
	UPROPERTY(BlueprintReadOnly,Category=PlayerAnim)
	bool isInAir = false;
};
