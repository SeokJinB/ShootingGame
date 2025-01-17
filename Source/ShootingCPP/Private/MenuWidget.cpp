// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"

void UMenuWidget::Restart()
{
	// 레벨 다시 로드
	UGameplayStatics::OpenLevel(GetWorld(), "ShootingMap");
}

void UMenuWidget::Quit()
{
	// 현재 실행 중인 월드 오브젝트를 가져오기
	UWorld* currentWorld = GetWorld();

	// 앱 종료
	UKismetSystemLibrary::QuitGame(currentWorld, currentWorld->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 각 버튼 입력 이벤트에 대응하는 함수 연결
	button_Restart->OnClicked.AddDynamic(this, &UMenuWidget::Restart);
	button_Quit->OnClicked.AddDynamic(this, &UMenuWidget::Quit);
}
