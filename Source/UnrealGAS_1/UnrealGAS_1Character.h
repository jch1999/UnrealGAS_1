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

