#include "MyPlayerController.h"
#include "MyHUD.h"
#include "HUDWidget.h"
#include "UnrealGAS_1\UnrealGAS_1Character.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	GetHudTryCnt = 0;
	SetHudWidget();
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	GetHudTryCnt = 0;
	SetHudWidget();
}

void AMyPlayerController::SetHudWidget()
{
	// if it isn't my own controller, don't create it.
	if (!IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
		return;
	}
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(Hud);
		if (IsValid(MyHud))
		{
			APawn* MyPawn = GetCharacter();

			MyHud->CreateHUD();
			MyHud->SpawnPlayerStateSetting(Cast<AUnrealGAS_1Character>(MyPawn));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnRep_PlayerState"));
		}
	}
	else
	{
		if (GetHudTryCnt++ < 20)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerForGetHUD, this, &AMyPlayerController::SetHudWidget, 0.2f, false, 0.2f);
		}
	}
}

//void AMyPlayerController::CreateHUD()
//{
//	// if it already exsit, don't operate.
//	if (UIHUDWidget != nullptr)
//	{
//		return;
//	}
//
//	// Must have origin class, which put in the editor.
//	if (UIHudWidgetClass == nullptr)
//	{
//		// Show Error msg with Function name.
//		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass."), *FString(__FUNCTION__));
//		return;
//	}
//
//	// Create widget
//	UIHUDWidget = CreateWidget<UHUDWidget>(GetWorld(), UIHudWidgetClass);
//	// Put it in a viewport and print it out on the screen
//	UIHUDWidget->AddToViewport();
//
//}
//
//UHUDWidget* AMyPlayerController::GetHUD()
//{
//	return UIHUDWidget;
//}
//
//void AMyPlayerController::SpawnPlayerStateSetting(AUnrealGAS_1Character* player)
//{
//	if (player && UIHUDWidget)
//	{
//		UIHUDWidget->SetPlayer(player);
//		UIHUDWidget->SetCurrentHealth(player->GetHealth());
//		UIHUDWidget->SetMaxHealth(player->GetMaxHealth());
//	}
//}

