// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAS_1_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// BlueprintImplementableEvent - Implementing Code in Blueprint
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void SetPlayer(class AUnrealGAS_1Character* player);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetCurrentHealth(float CurrentHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetMaxHealth(float MaxHealth);
};
