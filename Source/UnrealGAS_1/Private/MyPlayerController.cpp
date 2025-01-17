// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "UnrealGAS_1\UnrealGAS_1Character.h"

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (!IsValid(aPawn))
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, aPawn]()
			{
				OnPossess(aPawn);
			});
		return;
	}
	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(Hud);
		if (IsValid(MyHud))
		{
			// if it isn't my own controller, don't create it.
			if (!IsLocalController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
				return;
			}

			MyHud->CreateHUD();
			MyHud->SpawnPlayerStateSetting(Cast<AUnrealGAS_1Character>(aPawn));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnPossess"));
		}
	}
}

void AMyPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();


	AHUD* Hud = GetHUD();
	if (IsValid(Hud))
	{
		AMyHUD* MyHud = Cast<AMyHUD>(Hud);
		if (IsValid(MyHud))
		{
			// if it isn't my own controller, don't create it.
			if (!IsLocalController())
			{
				UE_LOG(LogTemp, Warning, TEXT("Is not local controller"));
				return;
			}
			APawn* MyPawn = GetCharacter();
			if (!IsValid(MyPawn))
			{
				UE_LOG(LogTemp, Warning, TEXT("Pawn is not valid in OnRep_PlayerState, delaying..."));

				// Pawn이 유효하지 않은 경우 일정 시간 후 다시 확인
				GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
					{
						OnRep_PlayerState();
					});
				return;
			}
			MyHud->SpawnPlayerStateSetting(Cast<AUnrealGAS_1Character>(MyPawn));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HUD is not valid in OnRep_PlayerState"));
		}
	}
}
