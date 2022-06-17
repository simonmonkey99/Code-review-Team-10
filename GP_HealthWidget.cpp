// Fill out your copyright notice in the Description page of Project Settings.


#include "GP_HealthWidget.h"
#include <Components/ProgressBar.h>

void UGP_HealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void UGP_HealthWidget::UpdateHealth(float CurHealth, float MaxHealth)
{
	HealthBar->SetPercent(CurHealth / MaxHealth);
}

