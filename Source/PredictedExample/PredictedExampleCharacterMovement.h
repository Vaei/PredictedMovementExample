// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stamina/StaminaMovement.h"
#include "PredictedExampleCharacterMovement.generated.h"

// EXAMPLE_CHANGE - Create a character movement component from UStaminaMovement

class APredictedExampleCharacter;
/**
 * 
 */
UCLASS()
class PREDICTEDEXAMPLE_API UPredictedExampleCharacterMovement : public UStaminaMovement
{
	GENERATED_BODY()

public:
	// EXAMPLE_CHANGE Properties to customize stamina drain and recovery

	UPROPERTY(Category="Character Movement: Stamina", EditAnywhere, BlueprintReadWrite)
	float SprintDrainRate;

	UPROPERTY(Category="Character Movement: Stamina", EditAnywhere, BlueprintReadWrite)
	float StaminaRegenRate;

	UPROPERTY(Category="Character Movement: Stamina", EditAnywhere, BlueprintReadWrite)
	float DrainedRegenRate;

protected:
	/** Character movement component belongs to */
	UPROPERTY(Transient, DuplicateTransient)
	TObjectPtr<APredictedExampleCharacter> ExampleCharacter;
	
public:
	UPredictedExampleCharacterMovement(const FObjectInitializer& ObjectInitializer);

	virtual void PostLoad() override;
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual bool HasValidData() const override;

	virtual void BeginPlay() override;

// Stamina & Sprint
	virtual void OnStaminaChanged(float PrevValue, float NewValue) override;
	virtual void OnMaxStaminaChanged(float PrevValue, float NewValue) override;
	virtual void OnStaminaDrained() override;
	virtual void OnStaminaDrainRecovered() override;

	virtual float GetMaxSpeed() const override;
	
	void CalcStamina(float DeltaTime);
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;

	void DebugStamina() const;

	// Prone
	virtual bool CanProneInCurrentState() const override;
};
