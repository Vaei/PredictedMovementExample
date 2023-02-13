// Fill out your copyright notice in the Description page of Project Settings.


#include "PredictedExampleCharacterMovement.h"

#include "PredictedExampleCharacter.h"
#include "GameFramework/Character.h"

UPredictedExampleCharacterMovement::UPredictedExampleCharacterMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// EXAMPLE_CHANGE Define max stamina

	SetMaxStamina(100.f);

	// EXAMPLE_CHANGE Make sprinting very noticable
	MaxAccelerationSprinting = 2048.f;
	MaxWalkSpeedSprinting = 1200.f;
	
	// EXAMPLE_CHANGE Enable crouching

	GetNavAgentPropertiesRef().bCanCrouch = true;

	SprintDrainRate = 20.f;
	StaminaRegenRate = 14.f;
	DrainedRegenRate = 4.f;
}

void UPredictedExampleCharacterMovement::PostLoad()
{
	Super::PostLoad();

	ExampleCharacter = Cast<APredictedExampleCharacter>(PawnOwner);
}

void UPredictedExampleCharacterMovement::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	Super::SetUpdatedComponent(NewUpdatedComponent);

	ExampleCharacter = Cast<APredictedExampleCharacter>(PawnOwner);
}

bool UPredictedExampleCharacterMovement::HasValidData() const
{
	return IsValid(ExampleCharacter) && Super::HasValidData();
}

void UPredictedExampleCharacterMovement::BeginPlay()
{
	Super::BeginPlay();

	// EXAMPLE_CHANGE Initialize Stamina and MaxStamina, set to 0 first so that it will trigger the change events
	// which will be important for updating any UI in particular later
	
	const float TempMaxStamina = GetMaxStamina();
	SetMaxStamina(0.f);
	SetMaxStamina(TempMaxStamina);

	SetStamina(0.f);
	SetStamina(GetMaxStamina());
}

void UPredictedExampleCharacterMovement::OnStaminaChanged(float PrevValue, float NewValue)
{
	// EXAMPLE_CHANGE Notify Character for purpose of UI
	// Either super or our own replacement logic is required here for handling drain state!

	if (IsValid(ExampleCharacter))
	{
		ExampleCharacter->OnStaminaChanged.Broadcast(NewValue);
	}

	// We will replace the Super call with it's own logic, but modified to require only 10% stamina for recovery
	// instead of 100%, as well as using Getters due to private accessibility of MaxStamina and bIsStaminaDrained

	// The comments from the original function have been retained for your perusal, but the suggestions have already
	// been implemented

	if (FMath::IsNearlyZero(Stamina))
	{
		Stamina = 0.f;
		if (!IsStaminaDrained())
		{
			SetStaminaDrained(true);
		}
	}
	// This will need to change if not using MaxStamina for recovery, here is an example (commented out) that uses
	// 10% instead; to use this, comment out the existing else if statement, and change the 0.1f to the percentage
	// you want to use (0.1f is 10%)
	else if (IsStaminaDrained() && Stamina >= GetMaxStamina() * 0.1f)
	{
		SetStaminaDrained(false);
	}
	
	// Super::OnStaminaChanged(PrevValue, NewValue);
}

void UPredictedExampleCharacterMovement::OnMaxStaminaChanged(float PrevValue, float NewValue)
{
	// EXAMPLE_CHANGE Notify Character for purpose of UI
	// No need for super, it doesn't do anything

	if (IsValid(ExampleCharacter))
	{
		if (ExampleCharacter->OnMaxStaminaChanged.IsBound())
		{
			ExampleCharacter->OnMaxStaminaChanged.Broadcast(NewValue);
		}
	}
}

void UPredictedExampleCharacterMovement::OnStaminaDrained()
{
	// EXAMPLE_CHANGE We must end sprinting here, otherwise it cannot end between ticks and we lose the benefit of the
	// additional accuracy between frames

	// No need for super, it doesn't do anything

	UnSprint(false);

	// EXAMPLE_CHANGE Notify Character for purpose of UI
	
	if (IsValid(ExampleCharacter))
	{
		if (ExampleCharacter->OnStaminaDrained.IsBound())
		{
			ExampleCharacter->OnStaminaDrained.Broadcast();
		}
	}
}

void UPredictedExampleCharacterMovement::OnStaminaDrainRecovered()
{
	// EXAMPLE_CHANGE Notify Character for purpose of UI
	// No need for super, it doesn't do anything

	if (IsValid(ExampleCharacter))
	{
		if (ExampleCharacter->OnStaminaRecovered.IsBound())
		{
			ExampleCharacter->OnStaminaRecovered.Broadcast();
		}
	}
}

float UPredictedExampleCharacterMovement::GetMaxSpeed() const
{
	// EXAMPLE_CHANGE lets punish players who end up in a drained state by making them move really slow until they
	// recover; you could even return 0.f and leave them stunned!
	
	// An animation instance could also retrieve IsDrained() to transition to a stunned animation state!

	return Super::GetMaxSpeed() * (IsStaminaDrained() ? 0.1f : 1.f);
}

void UPredictedExampleCharacterMovement::CalcStamina(float DeltaTime)
{
	// EXAMPLE_CHANGE Drain and Restore stamina based on sprint state

	// You do not need to check if a change can actually occur, and you do not need to do any clamping, thats already
	// done for you
	
	if (IsSprintingAtSpeed())
	{
		SetStamina(GetStamina() - SprintDrainRate * DeltaTime);
	}
	else
	{
		// We can optioanlly allow faster or slower recovery based on the needs of the project once stamina has been
		// drained
		const float RegenRate = IsStaminaDrained() ? DrainedRegenRate : SprintDrainRate;
		SetStamina(GetStamina() + RegenRate * DeltaTime);
	}

	// Draw some debug values to the screen
	DebugStamina();
}

void UPredictedExampleCharacterMovement::CalcVelocity(float DeltaTime, float Friction, bool bFluid,
	float BrakingDeceleration)
{
	// EXAMPLE_CHANGE Call CalcStamina

	// CalcVelocity occurs between subticks, it is highly accurate and can end sprinting between frames
	
	CalcStamina(DeltaTime);
	Super::CalcVelocity(DeltaTime, Friction, bFluid, BrakingDeceleration);
}

void UPredictedExampleCharacterMovement::DebugStamina() const
{
#if !UE_BUILD_SHIPPING
	if (GEngine)
	{
		if (CharacterOwner->HasAuthority())
		{
			GEngine->AddOnScreenDebugMessage(38265, 1.f, FColor::Orange, FString::Printf(TEXT("[Authority] Stamina %f    Drained %d"), GetStamina(), IsStaminaDrained()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(38266, 1.f, FColor::Orange, FString::Printf(TEXT("[Autonomous] Stamina %f    Drained %d"), GetStamina(), IsStaminaDrained()));
		}
	}
#endif
}

bool UPredictedExampleCharacterMovement::CanProneInCurrentState() const
{
	// EXAMPLE_CHANGE Can only prone when on the ground, saves us handling more complex animation transitions! (which
	// we don't actually do for this example either way)

	if (!IsMovingOnGround())
	{
		return false;
	}
	return Super::CanProneInCurrentState();
}
