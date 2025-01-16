// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAbilitySystemComponent.h"
#include "AsyncTaskAttributeChanged.generated.h"

// Delegate that has three parameters. when attribute value changed, give changed GameplayAttribute, old value and new value.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

// Assign listener automatically in blueprint node, and calls when you change the attribute value
// UBlueprintAsyncActionBase is the type that can be called from Blueprint at any time.
UCLASS()
class UNREALGAS_1_API UAsyncTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// Can connect function in blueprint
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged; // Function Pointer


	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly=true))
	static UAsyncTaskAttributeChanged* ListenForAttriuteChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);
	
	// Get multiple attribute values;
	static UAsyncTaskAttributeChanged* ListenForAttriutesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

	// Finsh the something created asynchronously
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor; // just one value can put
	TArray<FGameplayAttribute> AttributesToListenFor;

	// Give changed Info, When value changed.
	void AttributeChanged(const FOnAttributeChangeData& Data);
};
