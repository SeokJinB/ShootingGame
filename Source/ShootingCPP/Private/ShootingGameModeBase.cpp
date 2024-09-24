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
		// mainWidget �������Ʈ ������ �޸𸮿� �ε�
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// ������ �޸𸮿� �ε�Ǹ� �� ��Ʈ�� ���
		if (mainUI != nullptr)
		{
			mainUI->AddToViewport();
		}
	}
}
// ���� ���� ��� �Լ�
void AShootingGameModeBase::AddScore(int32 point)
{
	// point�� ���� �Ѱܹ��� ���� ����
	currentScore += point;

	// ���� ������ ������ �ݿ�
	PrintScore();
}

void AShootingGameModeBase::PrintScore()
{
	if (mainUI != nullptr)
	{
		// scoreData �ؽ�Ʈ ��Ͽ� ���� ���� ���� �Է�
		mainUI->scoreData->SetText(FText::AsNumber(currentScore));
	}
}

void AShootingGameModeBase::ShowMenu()
{
	if (menuWidget != nullptr)
	{
		// �޴� ���� ����
		menuUI = CreateWidget<UMenuWidget>(GetWorld(), menuWidget);

		if (menuUI != nullptr)
		{
			// ������ �޴� ������ �� ��Ʈ�� ���
			menuUI->AddToViewport();

			// ���� �Ͻ� ����
			UGameplayStatics::SetGamePaused(GetWorld(), true);

			// �÷��̾� ��Ʈ�ѷ����� ���콺 Ŀ���� ȭ�鿡 ���̰� ����
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		}
	}
}