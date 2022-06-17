// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_InGameHUD.h"
#include "Components/WidgetComponent.h"
#include "GP_HealthWidget.h"

AGP_InGameHUD::AGP_InGameHUD()
{
}

void AGP_InGameHUD::BeginPlay()
{
	Super::BeginPlay();

	
}

void AGP_InGameHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGP_InGameHUD::CreateHealthWidget()
{
	if (HealthWidgetClass)
	{
		HealthWidget = CreateWidget<UGP_HealthWidget>(GetWorld(), HealthWidgetClass);

		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
		}
	}
}


void AGP_InGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AGP_InGameHUD::UpdateHealth(float CurHealth, float MaxHealth)
{
	if (HealthWidget)
	{
		HealthWidget->UpdateHealth(CurHealth, MaxHealth);
	}
}
