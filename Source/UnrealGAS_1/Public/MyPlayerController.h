// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS_1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	// Execute When Poseess
	virtual void OnPossess(APawn* aPawn) override;

	// Execute Whe Player State Changed
	virtual void OnRep_PlayerState() override;

private:
	void SetHudWidget();

public:
	void CreateHUD();
	class UHUDWidget* GetHUD();
	void SpawnPlayerStateSetting(class AUnrealGAS_1Character* player);

protected:
	// TSubclassOf UHUDWidget�� ��ӹ��� Ŭ������ �巡�� �����ϰ�
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|UI")
	TSubclassOf<class UHUDWidget> UIHudWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "RPG|UI")
	class UHUDWidget* UIHUDWidget; // ���� ������ �������� ������ UI

private:
	FTimerHandle TimerForGetHUD;
	float GetHudTryCnt;
};
