// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜라이더 컴포넌트 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));

	// 생성한 박스 콜라이더 컴포넌트를 최상단 컴포넌트로 설정
	SetRootComponent(boxComp);

	// 스태틱 메시 컴포넌트 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	// 박스 콜라이더 컴포넌트의 자식 컴포넌트로 설정
	meshComp->SetupAttachment(boxComp);

	// 박스 콜라이더 크기 50x50x50으로 설정
	FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxSize);

	// 총구 표시 컴포넌트를 생성하고 박스 컴포넌트의 자식으로 설정
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	firePosition->SetupAttachment(boxComp);

	// 오버랩 이벤트 on
	boxComp->SetGenerateOverlapEvents(true);

	// 충돌 응답을 Query And Physics로 설정
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Object Type을 1번 채널(Player)로 설정
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// 모든 채널을 충돌 응답 없음으로 설정
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Enemy와는 충돌 이벤트 체크
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// 현재 플레이어가 소유한 컨트롤러를 가져온다
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	// 만약 플레이어 컨트롤러 변수에 값이 있다면
	if (pc != nullptr)
	{
		// 플레이어 컨트롤러로부터 입력 서브 시스템을 가져온다
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// 입력 서브 시스템에 IMC 파일 변수를 연결
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 사용자의 입력 키를 이용해서
	// 1. 상하 입력 값과 좌우 입력 값으로 방향벡터 생성
	FVector dir = FVector(0, h, v);

	// 2. 방향 벡터의 길이가 1이 되도록 정규화
	dir.Normalize();

	// 3. 이동할 위치 좌표
	FVector newLocation = GetActorLocation() + (dir * moveSpeed * DeltaTime);

	// 4. 현재 액터의 위치 좌표를 앞에서 구한 새 좌표로 갱신
	SetActorLocation(newLocation, true);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputHorizontal);
		enhancedInputComponent->BindAction(ia_horizontal, ETriggerEvent::Completed, this, &APlayerPawn::OnInputHorizontal);
		enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Triggered, this, &APlayerPawn::OnInputVertical);
		enhancedInputComponent->BindAction(ia_vertical, ETriggerEvent::Completed, this, &APlayerPawn::OnInputVertical);
		enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Started, this, &APlayerPawn::Fire);
	}
}

// 좌우축 입력 처리 함수
void APlayerPawn::OnInputHorizontal(const FInputActionValue& value)
{
	// 로그 출력 코드
	/*float hor = value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT(" Horizontal: %.2f "), hor);*/

	h = value.Get<float>();
}

// 상하축 입력 처리 함수
void APlayerPawn::OnInputVertical(const FInputActionValue& value)
{
	// 로그 출력 코드
	/*float ver = value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT(" Vertical: %.2f "), ver);*/

	v = value.Get<float>();
}

// 마우스 왼쪽 버튼 입력 처리 함수
void APlayerPawn::Fire()
{
	// 총알 블루프린트 파일을 firePosition 위치에 생성
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), firePosition->GetComponentRotation());

	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}
