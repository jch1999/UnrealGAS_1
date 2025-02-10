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
//protected:
//	//�����Ϳ��� TSubclassOf �ش� �ڷ��� Ŭ������ �巡�� �����ϰ�
//	//�츮�� �����Ϳ� �־��� UIŬ���� ����.
//	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|UI")
//	TSubclassOf<class UHUDWidget> UIHUDWidgetClass;
//
//	UPROPERTY(BlueprintReadWrite, Category = "RPG|UI")
//	class UHUDWidget* UIHUDWidget;//���� �������� ������ UI
//
//public:
//	class UHUDWidget* GetHUD();
//	void CreateHUD();
//
//	UFUNCTION()
//	void SpawnPlayerStateSetting(class AUnrealGAS_1Character* _Player);
	FTimerHandle TimerForGetHUD;
	float GetHudTryCnt;
};
