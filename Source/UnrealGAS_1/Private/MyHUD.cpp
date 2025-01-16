// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "HUDWidget.h"
#include "UnrealGAS_1\UnrealGAS_1Character.h"

void AMyHUD::CreateHUD()
{
	// if it already exsit, don't operate.
	if (UIHUDWidget != nullptr)
	{
		return;
	}

	// Must have origin class, which put in the editor.
	if (UIHudWidgetClass == nullptr)
	{
		// Show Error msg with Function name.
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass."), *FString(__FUNCTION__));
		return;
	}

	// Create widget
	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHudWidgetClass);
	// Put it in a viewport and print it out on the screen
	UIHUDWidget->AddToViewport();

}

UHUDWidget* AMyHUD::GetHUD()
{
	return UIHUDWidget;
}

void AMyHUD::SpawnPlayerStateSetting(AUnrealGAS_1Character* player)
{
	if (player && UIHUDWidget)
	{
		UIHUDWidget->SetPlayer(player);
		UIHUDWidget->SetCurrentHealth(player->GetHealth());
		UIHUDWidget->SetMaxHealth(player->GetMaxHealth());
	}
}

