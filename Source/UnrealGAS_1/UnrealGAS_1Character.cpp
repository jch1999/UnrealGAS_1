// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealGAS_1Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUnrealGAS_1Character

AUnrealGAS_1Character::AUnrealGAS_1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create WidgetComponent
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComp->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Create Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UMyAbilitySystemComponent>(TEXT("ABilitysystemComponent"));

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUnrealGAS_1Character::BeginPlay()
{

	if (HeadUpWidgetClass != nullptr)
	{
		//if (Cast<APlayerController>(GetInstigatorController()))
		//{
			WidgetComp->SetWidgetClass(HeadUpWidgetClass);
			WidgetComp->SetRelativeLocation(FVector(0, 0, 120.0f));
			WidgetComp->SetDrawSize(FVector2D(240, 30));
			WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		//}
	}

	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		// Get DataAsset as UMyAttributeSet
		AttributeSetVar = AbilitySystemComponent->GetSet<UMyAttributeSet>();
		if (AttributeSetVar != nullptr)
		{
			const_cast<UMyAttributeSet*>(AttributeSetVar)->OnHealthChangeDelegate.AddDynamic(this, &AUnrealGAS_1Character::OnHealthChangeNative);

			InitializeAttribute();
			AddStartUpEffects();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing AbilitySystemCopmonent."), *FString(__FUNCTION__));
	}
}

void AUnrealGAS_1Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AUnrealGAS_1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealGAS_1Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnrealGAS_1Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

UMyAbilitySystemComponent* AUnrealGAS_1Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AUnrealGAS_1Character::ChangeAbilityLevelWithTags(FGameplayTagContainer TagContiner, int32 Level)
{
	// Delete Multiple Tags
	TArray<struct FGameplayAbilitySpec*> MatchingAbilities;
	// Compare the tags currently have and get them if they match the containers put in as parameters
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContiner, MatchingAbilities, true);
	for (FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		Spec->Level = Level;
	}
}

void AUnrealGAS_1Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		// Deliver to call actors that use the system from the availability system
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		//Set the level of all skills set in the editor to 1
		InitializeAbilityMulti(InitialAbilities, 1);
	}
}

void AUnrealGAS_1Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For using the right actor on the server, need to set it up here as well.
	if (IsValid(AbilitySystemComponent))
	{
		// Deliver to call actors that use the system from the availability system
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		// Set the level of all skills set in the editor to 1
		InitializeAbilityMulti(InitialAbilities, 1);
	}
}

void AUnrealGAS_1Character::InitializeAttribute()
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing AbilitySystemCopmonent."), *FString(__FUNCTION__));
		return;
	}
	if (!IsValid(DefaultAttributes))
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes."), *FString(__FUNCTION__));
		return;
	} 

	// Create Effect Handle
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // Set Target to Apply.

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0, EffectContext);
	if (NewHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void AUnrealGAS_1Character::AddStartUpEffects()
{
	if (!IsValid(AbilitySystemComponent) || 
		GetLocalRole() != ROLE_Authority || 
		AbilitySystemComponent->StartUpEffectSupplied)
	{
		return;
	}

	// Create Effect Handle
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this); // Set Target to Apply.

	
	for (TSubclassOf<class UGameplayEffect> GameplayEffect : StartUpEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 0, EffectContext);

		if (NewHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->StartUpEffectSupplied = true;
}

void AUnrealGAS_1Character::InitializeAbility(TSubclassOf<class UGameplayAbility> AbilityToGet, int32 AbilityLevel)
{
	// Add availability only when it's a server, meaningless if it's not a server
	if (HasAuthority())
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityToGet, AbilityLevel));
	}
}

void AUnrealGAS_1Character::InitializeAbilityMulti(TArray<TSubclassOf<class UGameplayAbility>> AbilityToAcquire, int32 AbilityLevel)
{
	// Add availability only when it's a server, meaningless if it's not a server
	if (HasAuthority())
	{
		// Add them all one by one
		for (TSubclassOf<class UGameplayAbility>& AbilityItem : AbilityToAcquire)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityItem, AbilityLevel));
		}
	}
}

void AUnrealGAS_1Character::RemoveAbilityWithTags(FGameplayTagContainer TagContiner)
{
	// Delete Multiple Tags
	TArray<struct FGameplayAbilitySpec*> MatchingAbilities;
	// Compare the tags currently have and get them if they match the containers put in as parameters
	AbilitySystemComponent->GetActivatableGameplayAbilitySpecsByAllMatchingTags(TagContiner, MatchingAbilities, true);
	for (FGameplayAbilitySpec* Spec : MatchingAbilities)
	{
		AbilitySystemComponent->ClearAbility(Spec->Handle);
	}
}

void AUnrealGAS_1Character::CancelAbilityWithTag(FGameplayTagContainer WithTag, FGameplayTagContainer WithoutTag)
{
	AbilitySystemComponent->CancelAbilities(&WithTag, &WithoutTag, nullptr);
}

void AUnrealGAS_1Character::AddLooseGamePlayTag(FGameplayTag TagToAdd)
{
	AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	AbilitySystemComponent->SetTagMapCount(TagToAdd, 1);
}

void AUnrealGAS_1Character::RemoveLooseGamePlayTag(FGameplayTag TagToAdd)
{
	AbilitySystemComponent->RemoveLooseGameplayTag(TagToAdd);
}

void AUnrealGAS_1Character::OnHealthChangeNative(float Health, int32 StackCount)
{
	// BlueprintImposableEvent function, so call it here, it's also called in Blueprint.
	OnHealthChanged(Health, StackCount);
	if (Health <= 0)
	{
		//Dead.
		Die();
	}
}

void AUnrealGAS_1Character::HealthValues(float& Health, float& MaxHealth)
{
	if (IsValid(AttributeSetVar))
	{
		Health = AttributeSetVar->GetHealth();
		MaxHealth = 1000.0f; // Temporary value
	}
}

float AUnrealGAS_1Character::GetHealth() const
{
	if (IsValid(AttributeSetVar))
	{
		return AttributeSetVar->GetHealth();
	}
	return 0.0f;
}

float AUnrealGAS_1Character::GetMaxHealth() const
{
	if (IsValid(AttributeSetVar))
	{
		return 1000.0f; // Temporary value
	}
	return 0.0f;
}

void AUnrealGAS_1Character::Die()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->Velocity = FVector(0.0f);

	if (IsValid(AbilitySystemComponent))
	{
		// Cancel All Active Abiliities
		AbilitySystemComponent->CancelAbilities();

		// Add Die Tag on Player
		FGameplayTag DieEffectTag = FGameplayTag::RequestGameplayTag(FName("Die"));
		

		FGameplayTagContainer gameplayTag{ DieEffectTag };

		// Check if Tags are attached, execute it.
		bool IsSuccess = AbilitySystemComponent->TryActivateAbilitiesByTag(gameplayTag);
		if (IsSuccess == false) // if these are not attached, just attach the tag.
		{
			AbilitySystemComponent->AddLooseGameplayTag(DieEffectTag);
			FinishDying();
		}
	}
}

void AUnrealGAS_1Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUnrealGAS_1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
