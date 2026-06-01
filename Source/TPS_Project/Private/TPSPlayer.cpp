// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Project/Public/TPSPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// TPS 카메라를 SpringArm 컴포넌트에 부착
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	springArmComp->SetupAttachment(RootComponent);// 계층 구조상 캡슐컴포넌트가 ROOT
	springArmComp->SetRelativeLocation(FVector(.0f,70.0f,90.0f)); // 암 컴포넌트의시작점
	springArmComp->TargetArmLength=400.0f;
	
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	cameraComp->SetupAttachment(springArmComp);
	
	// C++에서 BP에서의 옵션을 직접 수정하는 경우 아래처럼 해당 옵션 변수들을 직접 코드로 제어 가능
	// springArmComp->bUsePawnControlRotation =true;
	// cameraComp->bUsePawnControlRotation=false;
	// bUseControllerRotationYaw=true;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// Engaced Input 시스템이 IMC_TPS 사용하도록 설정
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(Imc_TPS,0);
		}
	}
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 플레이어 이동 처리
	// P결과 위치 = P0초기위치 +V속도*t시간
	FVector P0 =GetActorLocation();
	FVector vt =direction*walkSpeed*DeltaTime;
	FVector P =P0+vt;
	SetActorLocation(P);
	
	direction=FVector::ZeroVector;
	
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	auto PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		PlayerInput->BindAction(ia_LooUP,ETriggerEvent::Triggered,this,&ATPSPlayer::LookUP);
		PlayerInput->BindAction(ia_Turn,ETriggerEvent::Triggered,this,&ATPSPlayer::Turn);
		PlayerInput->BindAction(ia_Move,ETriggerEvent::Triggered,this,&ATPSPlayer::Move);
	}
}



// 상하 회전 입력에 따른 롤백 함수 구현
void ATPSPlayer::LookUP(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerPitchInput(value); //PITCH(Y축) 회전
	
}

// 좌우 회전 입력에 따른 롤백 함수 구현
void ATPSPlayer::Turn(const FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	AddControllerYawInput(value); // PITCH(X축) 회전
	
}

// 전우좌우 이동 입력에 따른 롤백 함수 구현
void ATPSPlayer::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>(); //전달 받는 2D 값
	direction.X=value.X;//전후
	direction.Y=value.Y;//좌우
}