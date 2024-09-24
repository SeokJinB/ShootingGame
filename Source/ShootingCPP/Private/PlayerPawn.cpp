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

	// �ڽ� �ݶ��̴� ������Ʈ ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));

	// ������ �ڽ� �ݶ��̴� ������Ʈ�� �ֻ�� ������Ʈ�� ����
	SetRootComponent(boxComp);

	// ����ƽ �޽� ������Ʈ ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	// �ڽ� �ݶ��̴� ������Ʈ�� �ڽ� ������Ʈ�� ����
	meshComp->SetupAttachment(boxComp);

	// �ڽ� �ݶ��̴� ũ�� 50x50x50���� ����
	FVector boxSize = FVector(50.0f, 50.0f, 50.0f);
	boxComp->SetBoxExtent(boxSize);

	// �ѱ� ǥ�� ������Ʈ�� �����ϰ� �ڽ� ������Ʈ�� �ڽ����� ����
	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Fire Position"));
	firePosition->SetupAttachment(boxComp);

	// ������ �̺�Ʈ on
	boxComp->SetGenerateOverlapEvents(true);

	// �浹 ������ Query And Physics�� ����
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// Object Type�� 1�� ä��(Player)�� ����
	boxComp->SetCollisionObjectType(ECC_GameTraceChannel1);

	// ��� ä���� �浹 ���� �������� ����
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	// Enemy�ʹ� �浹 �̺�Ʈ üũ
	boxComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� �÷��̾ ������ ��Ʈ�ѷ��� �����´�
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	// ���� �÷��̾� ��Ʈ�ѷ� ������ ���� �ִٸ�
	if (pc != nullptr)
	{
		// �÷��̾� ��Ʈ�ѷ��κ��� �Է� ���� �ý����� �����´�
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr)
		{
			// �Է� ���� �ý��ۿ� IMC ���� ������ ����
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������� �Է� Ű�� �̿��ؼ�
	// 1. ���� �Է� ���� �¿� �Է� ������ ���⺤�� ����
	FVector dir = FVector(0, h, v);

	// 2. ���� ������ ���̰� 1�� �ǵ��� ����ȭ
	dir.Normalize();

	// 3. �̵��� ��ġ ��ǥ
	FVector newLocation = GetActorLocation() + (dir * moveSpeed * DeltaTime);

	// 4. ���� ������ ��ġ ��ǥ�� �տ��� ���� �� ��ǥ�� ����
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

// �¿��� �Է� ó�� �Լ�
void APlayerPawn::OnInputHorizontal(const FInputActionValue& value)
{
	// �α� ��� �ڵ�
	/*float hor = value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT(" Horizontal: %.2f "), hor);*/

	h = value.Get<float>();
}

// ������ �Է� ó�� �Լ�
void APlayerPawn::OnInputVertical(const FInputActionValue& value)
{
	// �α� ��� �ڵ�
	/*float ver = value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT(" Vertical: %.2f "), ver);*/

	v = value.Get<float>();
}

// ���콺 ���� ��ư �Է� ó�� �Լ�
void APlayerPawn::Fire()
{
	// �Ѿ� �������Ʈ ������ firePosition ��ġ�� ����
	ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), firePosition->GetComponentRotation());

	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}
