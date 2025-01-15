// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS_1_API AMyHUD : public AHUD
{
	GENERATED_BODY()

protected:

	// TSubclassOf UHUDWidget�� ��ӹ��� Ŭ������ �巡�� �����ϰ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|UI")
	TSubclassOf<class UHUDWidget> UIHudWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "RPG|UI")
	class UHUDWidget* UIHUDWidget; // ���� ������ �������� ������ UI

public:
	void CreateHUD();
	class UHUDWidget* GetHUD();
	void SpawnPlayerStateSetting(class AUnrealGAS_1Character* player);
};
