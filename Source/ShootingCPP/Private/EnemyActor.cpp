// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "PlayerPawn.h"
#include "ShootingGameModeBase.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(boxComp);

	// Collision presets�� Enemy ���������� ����
	boxComp->SetCollisionProfileName(TEXT("Enemy"));
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 1~100 ������ ������ ���� �� ��÷
	int32 drawResult = FMath::RandRange(1, 100);

	// ��÷ ���� ���� Ȯ�� �������� �۰ų� ���� ��
	if (drawResult <= traceRate)
	{
		// ���� ������ APlayerPawn Ŭ������ �� ���͸� ��� �˻�
		for (TActorIterator<APlayerPawn> player(GetWorld()); player; ++player)
		{
			// �˻��� ������ �̸��� "BP_PlayerPawn"�̶� ������ ���ԵǾ� ������
			if (player->GetName().Contains(TEXT("BP_PlayerPawn")))
			{
				// �÷��̾��� ��ġ - �ڽ��� ��ġ
				dir = player->GetActorLocation() - GetActorLocation();
				dir.Normalize();
			}
		}
	}
	else
	{
		// ������ ���� ����
		dir = GetActorForwardVector();
	}

	// �ڽ� ������Ʈ�� BeginOverlap ��������Ʈ�� OnEnemyOverlap �Լ��� ����
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnEnemyOverlap);
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ��ġ ����
	FVector newLocation = GetActorLocation() + (dir * moveSpeed * DeltaTime);
	SetActorLocation(newLocation);
}

void AEnemyActor::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �浹�� ��� ���͸� APlayerPawn Ŭ������ ��ȯ
	APlayerPawn* player = Cast <APlayerPawn>(OtherActor);

	// ���� ĳ������ ���������� �۵��� ��
	if (player != nullptr)
	{
		// �ε��� ��� ����
		OtherActor->Destroy();

		// ���� ���� ��� ��������
		AShootingGameModeBase* currentGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());

		if (currentGameMode != nullptr)
		{
			// �޴� UI ���� �Լ� ȣ��
			currentGameMode->ShowMenu();
		}
	}

	// �ڱ��ڽ��� ����
	Destroy();
}
