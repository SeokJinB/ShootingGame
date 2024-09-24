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

	// �ڽ� ������Ʈ ũ�� ����
	boxComp->SetWorldScale3D(FVector(0.75f, 0.25f, 1.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(boxComp);

	// �ڽ� ������Ʈ�� Collision presets�� Bullet���� ����
	boxComp->SetCollisionProfileName(TEXT("Bullet"));
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
	// �ڽ� ������Ʈ�� �浹 ������ �̺�Ʈ�� BulletOverlap �Լ��� ����
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnBulletOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� �̵� ��ġ ���
	FVector newLocation = GetActorLocation() + (GetActorForwardVector() * moveSpeed * DeltaTime);

	// ��ġ ����
	SetActorLocation(newLocation);
}

void ABullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �浹�� ���͸� AEnemyActor Ŭ������ ��ȯ
	AEnemyActor* enemy = Cast<AEnemyActor>(OtherActor);

	// ���� ĳ������ ���������� �Ǿ��ٸ�
	if (enemy != nullptr)
	{
		// �浹�� ���� ����
		OtherActor->Destroy();

		// ���� ����Ʈ ����
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation(), GetActorRotation());

		// ���� ���� ��� ��������
		AGameModeBase* currentMode = GetWorld()->GetAuthGameMode();

		// AShootingGmaeModeBase Ŭ������ ��ȯ
		AShootingGameModeBase* currentGameModeBase = Cast<AShootingGameModeBase>(currentMode);

		// ���� ��� ���̽��� �����Դٸ� 1�� �߰�
		if (currentGameModeBase != nullptr)
		{
			currentGameModeBase->AddScore(1);
		}
	}

	Destroy();
}
