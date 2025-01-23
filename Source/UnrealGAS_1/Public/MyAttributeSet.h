#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MyAbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct FGAmeplayEffectModCallbackData;

// Deligates with two parameters, operating when properties change
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttributeChangeDelegate, float, Attr, int32, StackCount);

UCLASS()
class UNREALGAS_1_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMyAttributeSet();

	// The functinon placed in ReplicatedUsing will be used when Health valued changed.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;

	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health); // Create Function whitch manage Health.

	// Deligates to operate when current Health changes
	FAttributeChangeDelegate OnHealthChangeDelegate;

	// The functinon placed in ReplicatedUsing will be used when MaxHealth valued changed.
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData MaxHealth;

	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth); // Create Function whitch manage MaxHealth.

	// Deligates to operate when current MaxHealth changes
	FAttributeChangeDelegate OnMaxHealthChangeDelegate;

	// Set with Deligate, you need this to call delegate
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	virtual void OnRep_Health(const struct FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const struct FGameplayAttributeData& OldMaxHealth);
	
};
