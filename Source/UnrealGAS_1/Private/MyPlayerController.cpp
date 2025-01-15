// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "UnrealGAS_1\UnrealGAS_1Character.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// if it isn't my own controller, don't create it.
	if (!IsLocalController())
	{
		return;
	}
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
			MyHud->SpawnPlayerStateSetting(Cast<AUnrealGAS_1Character>(aPawn));
		}
	}
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// if it isn't my own controller, don't create it.
	if (!IsLocalController())
	{
		return;
	}

	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(Hud);
		if (IsValid(MyHud))
		{
			MyHud->CreateHUD();
			MyHud->SpawnPlayerStateSetting(Cast<AUnrealGAS_1Character>(aPawn));
		}
	}
}
