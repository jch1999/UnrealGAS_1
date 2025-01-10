#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

UCLASS()
class UNREALGAS_1_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	// Ensure that the initial effect has been applied only once
	bool StartUpEffectSupplied = false;
};
