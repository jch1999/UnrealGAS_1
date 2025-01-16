// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskAttributeChanged.h"

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttriuteChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute)
{
    // statict function. Whenever it called, create instance with same class.
    UAsyncTaskAttributeChanged* WaitForAttributeChangeTask = NewObject<UAsyncTaskAttributeChanged>();
    
    // Set Parameter 
    WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
    WaitForAttributeChangeTask->AttributeToListenFor = Attribute;

    if (!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
    {
        // Remove created object. It automatically removed later because it cretaed by unreal.
        WaitForAttributeChangeTask->RemoveFromRoot();
        return nullptr;
    }

    // Register the AttributeChanged function of this class to act asynchronously 
    // when changing properties in the AbilitySystemComponent.
    // Basic features provided by AbilitySystem Component
    AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
        Attribute).AddUObject(WaitForAttributeChangeTask, &UAsyncTaskAttributeChanged::AttributeChanged);

    return WaitForAttributeChangeTask;
}

UAsyncTaskAttributeChanged* UAsyncTaskAttributeChanged::ListenForAttriutesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes)
{
    // statict function. Whenever it called, create instance with same class.
    UAsyncTaskAttributeChanged* WaitForAttributeChangeTask = NewObject<UAsyncTaskAttributeChanged>();

    // Set Parameter 
    WaitForAttributeChangeTask->ASC = AbilitySystemComponent;
    WaitForAttributeChangeTask->AttributesToListenFor = Attributes;

    if (!IsValid(AbilitySystemComponent) || !(Attributes.Num() < 1))
    {
        // Remove created object. It automatically removed later because it cretaed by unreal.
        WaitForAttributeChangeTask->RemoveFromRoot();
        return nullptr;
    }

    // Register the AttributeChanged function of this class to act asynchronously 
    // when changing properties in the AbilitySystemComponent.
    // Basic features provided by AbilitySystem Component
    for (FGameplayAttribute Attribute : Attributes)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
            Attribute).AddUObject(WaitForAttributeChangeTask, &UAsyncTaskAttributeChanged::AttributeChanged);
    }
    return WaitForAttributeChangeTask;
}

void UAsyncTaskAttributeChanged::EndTask()
{
    if (IsValid(ASC))
    {
        ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);
    }

    // Remove created task.
    SetReadyToDestroy();
    MarkAsGarbage();
}

void UAsyncTaskAttributeChanged::AttributeChanged(const FOnAttributeChangeData& Data)
{
    OnAttributeChanged.Broadcast(Data.Attribute, Data.NewValue, Data.OldValue);
}
