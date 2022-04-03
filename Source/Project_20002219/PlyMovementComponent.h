#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlyMovementComponent.generated.h"

class APly;

UCLASS()
class PROJECT_20002219_API UPlyMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPlyMovementComponent();

public:
	APly* Player;

public:
	// WASD movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
		float Sensitivity;

	bool StrafeThreshhold;

	void Turn(float Value);

	void LookUp(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	// Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
		float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
		float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
		float JumpSpeed;

	void StartCrouch();

	void StopCrouch();

	void CrouchImpl(float DeltaTime, float EyeHeight, float Height);

	void Sprint();

	void StopSprinting();


protected:
	bool bIsCrouching;
	bool uncrouchBlocked;


public:
	// Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		class UStaminaComponent* StaminaComponent;

	UPROPERTY()
		FTimerHandle SprintTimer;

	void LoseStamina();

	void GainStamina();
};
