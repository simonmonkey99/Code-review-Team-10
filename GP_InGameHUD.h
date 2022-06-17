// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GP_InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class GP3_UPROJECT_API AGP_InGameHUD : public AHUD
{
	GENERATED_BODY()
	
public: 
	AGP_InGameHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void CreateHealthWidget();

	UFUNCTION()
	void UpdateHealth(float CurHealth, float MaxHealth);


	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> HealthWidgetClass;

private: 
	class UGP_HealthWidget* HealthWidget;

};
