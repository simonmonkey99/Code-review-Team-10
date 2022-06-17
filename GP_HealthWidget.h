// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GP_HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class GP3_UPROJECT_API UGP_HealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	void NativeConstruct() override;

public: 
	void UpdateHealth(float CurHealth, float MaxHealth);
};
