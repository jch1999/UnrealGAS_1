#include "MyAttributeSet.h"
#include "Net/UnrealNetwork.h" // Unreal Newtwork communication
#include "GameplayEffect.h" // Effects for conveying ability points, tags, etc. Buff, Debuff 
#include "GameplayEffectExtension.h" // for Extension

UMyAttributeSet::UMyAttributeSet()
{

}

void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
}

void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);
}

void UMyAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) // 체력이 변경된거면
	{
		// GetHealth() - Get Changed Value
		SetHealth(FMath::Clamp(GetHealth(), 0.1f, 1000.0f)); // Set Health between 0~1000
		OnHealthChangeDelegate.Broadcast(GetHealth(), Data.EffectSpec.StackCount);
	}
}
