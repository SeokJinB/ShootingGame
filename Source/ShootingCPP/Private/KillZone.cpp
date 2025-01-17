// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "Components/BoxComponent.h"

// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 컴포넌트 생성 후 루트 컴포넌트 설정
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	SetRootComponent(boxComp);

	// 박스 컴포넌트의 모빌리티를 고정 상태로 설정
	boxComp->SetMobility(EComponentMobility::Static);

	// 박스의 크기를 50, 2000, 50으로 설정
	boxComp->SetBoxExtent(FVector(50, 2000, 50));

	// KillZone을 박스 컴포넌트의 콜리전 프리셋으로 설정
	boxComp->SetCollisionProfileName(TEXT("KillZone"));
}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

