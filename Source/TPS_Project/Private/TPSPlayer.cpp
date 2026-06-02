// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Project/Public/TPSPlayer.h"

#include "Bullet.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

#include <iostream>


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
	
	// 총 스켈레탈 메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	// 캐릭터 메시 컴포넌트 (GetMesh()) 부모에 부착
	gunMeshComp->SetupAttachment(GetMesh());
	// 스켈레탈 메시 데이터 동적 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/GrenadeLauncher/Meshes/SKM_GrenadeLauncher.SKM_GrenadeLauncher'"));
	if (TempGunMesh.Succeeded())
	{
		// 해당 경로의 스켈레탈메시를 찾았다면, 메시 할당 + 임시 위치 보정
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14.0f,52.0,120.f));
	
	}
	// 스나이퍼건 스태틱 메시 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGUn StatickMeshComponent"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSniperGunMesh(TEXT("/Script/Engine.StaticMesh'/Game/Weapons/Sniper/Meshes/sniper1.sniper1'"));
	
	if (TempSniperGunMesh.Succeeded())
	{
		// 해당 경로의 스켈레탈메시를 찾았다면, 메시 할당 + 임시 위치 보정
		sniperGunComp->SetStaticMesh(TempSniperGunMesh.Object);
		sniperGunComp->SetRelativeLocation(FVector(-14.0f,52.0,120.f));
		
		sniperGunComp->SetRelativeScale3D(FVector(0.8f));
	}
	
	// 시작 시 기본 무기로 스나이퍼건을(유탄총을 숨김)
	bUsingGrenadeGun=false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(true);
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
	
	// 캐릭터 이동처리에서 버그예시)
	// GetControlRotaion() 함수는 좌우(YAW) 말고도 상하(PITCH) 까지 포함된 카메라 전체 회전
	// PITCH 움직임에 X축을 기울이는 성질이 있어서 카메라가 아래를 향하는 상태에서 W를 누르면
	// "앞으로 가는 입력" + "아래로 박는 입력"으로 섞임 -> 이때 수평 속도가 cos(Pitch)로줄어듬
	
	// 컨트롤러의 현재 회전 값들 (YAW,PITCH,ROLL) 값을 가져옴
	FRotator controlRot = GetControlRotation();
	// PITCH 자체를 0으로 - 기우는 현상 차단
	controlRot.Pitch = 0.0f;
	// ROLL 도 0으로 고정 - 기우는 현상 차단
	controlRot.Roll = 0.0f;
	
	// YAW좌우 속값만 남은 백터를 넣어둠 -> 카메라가 어디를 보던지 , 이동은 수평면 위에서만 이루어지도록 함
	direction=FRotationMatrix(controlRot).TransformVector(direction);
	
	// // 플레이어 이동 처리
	// // P결과 위치 = P0초기위치 +V속도*t시간
	// direction = FTransform(GetControlRotation()).TransformFVector4(direction); //자기를 기준으로 백턴 변환
	// FVector P0 =GetActorLocation();
	// FVector vt =direction*walkSpeed*DeltaTime;
	// FVector P =P0+vt;
	// SetActorLocation(P);
	
	// 언리얼 엔진에서 제공하는 위 등속 운동을 구현하는 함수 AddMovementInput()
	AddMovementInput(direction);
	
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
		PlayerInput->BindAction(ia_Jump,ETriggerEvent::Started,this,&ATPSPlayer::InputJump);
		PlayerInput->BindAction(ia_Fire,ETriggerEvent::Started,this,&ATPSPlayer::InputFire);
		PlayerInput->BindAction(ia_GranadeGun,ETriggerEvent::Started,this,&ATPSPlayer::ChangeToGrenadeGun);
		PlayerInput->BindAction(ia_SniperGun,ETriggerEvent::Started,this,&ATPSPlayer::ChangeToSniperGun);
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

void ATPSPlayer::InputJump(const FInputActionValue& inputValue)
{
	Jump();//ACharacter 클래스가 제공하는 기본 점프 함수 호출
}

void ATPSPlayer::InputFire(const FInputActionValue& inputValue)
{
	// 총 스켈레탈 메시에 FirePosition 이란 이름의 소켓의 월드 트랜스폼 (위치/회전)을 가져옴
	FTransform firePosiotion =  gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	// 위 위치/회전으로 BulletFactory가 BP_Bullet 인스턴스를 월드에 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory,firePosiotion);

	
}
void ATPSPlayer::ChangeToGrenadeGun(const FInputActionValue& inputValue)
{
	// 사용 중 플래그를 유탄총으로 변경
	bUsingGrenadeGun=true;
	//	스나이퍼 숨기고 / 유탄총 보이게
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun(const FInputActionValue& inputValue)
{
	// 사용 중 플래그를 유탄총으로 변경
	bUsingGrenadeGun=false;
	//	스나이퍼 숨기고 / 유탄총 보이게
	gunMeshComp->SetVisibility(false);
	sniperGunComp->SetVisibility(true);
}