// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Sprint/SprintCharacter.h"
#include "PredictedExampleCharacter.generated.h"

// EXAMPLE_CHANGE - UI Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaChanged, float, Stamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaxStaminaChanged, float, MaxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaDrained);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaRecovered);

class UPredictedExampleCharacterMovement;

UCLASS(config=Game)
class APredictedExampleCharacter : public ASprintCharacter  // EXAMPLE_CHANGE - Inherit ASprintCharacter (Stamina does not have a character)
{
	GENERATED_BODY()

	// EXAMPLE_CHANGE - Add our character movement component here
	
	/** Movement component used for movement logic in various movement modes (walking, falling, etc), containing relevant settings and functions to control movement. */
	UPROPERTY(Category=Character, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UPredictedExampleCharacterMovement> PredictedMovement;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	// EXAMPLE_CHANGE - UI Events
	
	UPROPERTY(BlueprintAssignable)
	FStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FMaxStaminaChanged OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FStaminaDrained OnStaminaDrained;

	UPROPERTY(BlueprintAssignable)
	FStaminaRecovered OnStaminaRecovered;

	// EXAMPLE_CHANGE - Stamina getters, the UI needs these
	
	UFUNCTION(BlueprintPure, Category="PredictedExample")
	float GetStamina() const;

	UFUNCTION(BlueprintPure, Category="PredictedExample")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category="PredictedExample")
	float GetStaminaNormalized() const;
	
public:
	/* EXAMPLE_CHANGE #2 - Add an object initializer */
	APredictedExampleCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

