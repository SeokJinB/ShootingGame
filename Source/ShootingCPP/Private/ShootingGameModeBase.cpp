// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"

void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mainWidget != nullptr)
	{
		// mainWidget 블루프린트 파일을 메모리에 로드
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// 위젯이 메모리에 로드되면 뷰 포트에 출력
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
		}
	}
}
// 현재 점수 계산 함수
void AShootingGameModeBase::AddScore(int32 point)
{
	// point를 통해 넘겨받은 점수 누적
	currentScore += point;

	// 현재 점수를 위젯에 반영
	PrintScore();
}

void AShootingGameModeBase::PrintScore()
{
	if (mainUI != nullptr)
	{
		// scoreData 텍스트 블록에 현재 점수 값을 입력
		mainUI->scoreData->SetText(FText::AsNumber(currentScore));
	}
}

void AShootingGameModeBase::ShowMenu()
{
	if (menuWidget != nullptr)
	{
		// 메뉴 위젯 생성
		menuUI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

		if (menuUI != nullptr)
		{
			// 생성한 메뉴 위젯을 뷰 포트에 출력
			menuUI->AddToViewport();

			// 게임 일시 정지
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// 플레이어 컨트롤러에서 마우스 커서를 화면에 보이게 설정
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}