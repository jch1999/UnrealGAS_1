// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

// Ability System
#include "MyAbilitySystemComponent.h"
// Attribute
#include "MyAttributeSet.h"

#include "UnrealGAS_1Character.generated.h"

class UMyAbilitySystemComponent;
class UMyAttributeSet;

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUnrealGAS_1Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AUnrealGAS_1Character();
	
public:
	// Add AbilitySystemComponent
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="GASGamePlayAbility")
	class UMyAbilitySystemComponent* AbilitySystemComponent;

	virtual class UMyAbilitySystemComponent* GetAbilitySystemComponent() const;

	// Character Info
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	const class UMyAttributeSet* AttributeSetVar;

	// Add a character's skill initially in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayAbility>> InitialAbilities;

	// Skill GameAbility, Set initial ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// Skill GameAbility, Standard, Health Recovery, Mana Recovery, etc
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGamePlayAbility")
	TArray<TSubclassOf<class UGameplayEffect>> StartUpEffects;

protected:
	// Inherited from the Character, Called when the player gained control of the character
	virtual void PossessedBy(AController* NewController) override;
	
	// Inherited from the Pawn, calling if character status has changed
	virtual void OnRep_PlayerState();

public: 
	// Function about skill.
	
	void InitializeAttribute();
	void AddStartUpEffects();

	// Initialize One Skill Ability
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitializeAbility(TSubclassOf<class UGameplayAbility> AbilityToGet, int32 AbilityLevel);

	// Initialize Multiple Skill Ability
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitializeAbilityMulti(TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel);

	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilitySkill")
	void InitializeAbilityMulti(TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel);

public:
	// Fuction about Game Ability Tag System
	
	// struct FGameplayTagContainer can contain multiple tags.
	
	// Delete Tag
	UFUNCTION(BlueprintCallable,Category="GASGamePlayAbilityTag")
	void RemoveAbilityWithTags(FGameplayTagContainer TagContiner);

	// Cancel Tag
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void CancelAbilityWithTag(FGameplayTagContainer WithTag, FGameplayTagContainer WithoutTag);

	// Add One Tag
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void AddLooseGamePlayTag(FGameplayTag TagToAdd);
	
	// Remove One Tag
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void RemoveLooseGamePlayTag(FGameplayTag TagToAdd);
	
	// Change Tag, Changing the tag level is similar to changing the skill level in a game
	UFUNCTION(BlueprintCallable, Category = "GASGamePlayAbilityTag")
	void ChangeAbilityLevelWithTags(FGameplayTagContainer TagContiner, int32 Level);

public:
	// Functions about Character Attributes
	// Called when Health changed
	UFUNCTION()
	void OnHealthChangeNative(float Health, int32 StackCount);

	// when the event occurs  in Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "GASGamePlayAbility")
	void OnHealthChanged(float Health, int32 StackCount);

	// Use to bring current Health, no pure implementation
	UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
	void HealthValues(float& Health, float& MaxHealth);

	// Just Get Call
	UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "GASGamePlayAbility")
	float GetMaxHealth() const;


protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

