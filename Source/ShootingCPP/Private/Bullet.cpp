// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGameModeBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	// 박스 컴포넌트 크기 변경
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	// 박스 컴포넌트의 Collision presets을 Bullet으로 설정
	boxComp->SetCollisionProfileName(TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// 박스 컴포넌트의 충돌 오버랩 이벤트에 BulletOverlap 함수를 연결
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 전방 이동 위치 계산
	FVector newLocation = GetActorLocation() + (GetActorForwardVector() * moveSpeed * DeltaTime);

	// 위치 갱신
	SetActorLocation(newLocation);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌한 액터를 AEnemyActor 클래스로 변환
	AEnemyActor* enemy = Cast<AEnemyActor>(OtherActor);

	// 만약 캐스팅이 정상적으로 되었다면
	if (enemy != nullptr)
	{
		// 충돌한 액터 제거
		OtherActor->Destroy();

		// 폭발 이펙트 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation(), GetActorRotation());

		// 현재 게임 모드 가져오기
		AGameModeBase* currentMode = GetWorld()->GetAuthGameMode();

		// AShootingGmaeModeBase 클래스로 변환
		AShootingGameModeBase* currentGameModeBase = Cast<AShootingGameModeBase>(currentMode);

		// 게임 모드 베이스를 가져왔다면 1점 추가
		if (currentGameModeBase != nullptr)
		{
			currentGameModeBase->AddScore(1);
		}
	}

	Destroy();
}
