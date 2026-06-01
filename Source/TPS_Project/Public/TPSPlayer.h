// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_PROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// 스프링 암 커포넌트 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class  USpringArmComponent* springArmComp;
	
	// 카메라 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* cameraComp;
	
	//IMC 선택 필드 선언
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputMappingContext* Imc_TPS ;
	
	// IA 선택 필드 선언
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* ia_LooUP;
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* ia_Turn;
	
	// 전후좌우 이동 IA 필드 선언
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction* ia_Move;
	
	// 키보드 점프 IA 필드 선언
	UPROPERTY(EditDefaultsOnly,Category="Input")
	class UInputAction*ia_Jump;
	
	// 이동 속도
	UPROPERTY(EditDefaultsOnly,Category=PlayerSetting)
	float walkSpeed = 600,f;
	
	// 이동 방향
	FVector direction;
	
	// 상하 회전 입력 함수 선언
	void LookUP(const struct FInputActionValue& inputValue);
	
	// 좌우 회전 입력 함수 선언
	void Turn(const struct FInputActionValue& inputValue);
	
	// 전후 좌우 이동 입력 함수 선언
	void Move(const struct FInputActionValue& inputValue);
	
	// 점프 입력 함수 선언
	void InputJump(const struct FInputActionValue& inputValue);
};
