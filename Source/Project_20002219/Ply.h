#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ply.generated.h"

UCLASS()
class PROJECT_20002219_API APly : public ACharacter
{
	GENERATED_BODY()

public:
	APly(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class USkeletalMeshComponent* PlyMesh;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* CameraComponent;

	UPROPERTY()
		class APlayerCameraManager* CameraManager;

	UPROPERTY()
		class APlayerController* PController;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Ammo")
		int32 DisplayMag();


public:
	// Visuals 
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> Crosshair;

	UPROPERTY()
		class UUserWidget* CrosshairInstance;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> UIBars;

	UPROPERTY()
		class UUserWidget* UIBarsInstance;


public:
	// Interact
	UPROPERTY()
		class AItem* HighlightItem;

	UPROPERTY()
		AActor* HoverActor;

	UPROPERTY()
		class UPhysicsHandleComponent* PhysicsHandle;

	void Use(AActor* actor);

	void RagdollUse(FVector camLoc, FRotator camRot);

	void StopUse();


public:
	// Ammo
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> UIAmmo;

	UPROPERTY()
		class UUserWidget* UIAmmoInstance;


public:
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		class UHealthComponent* HealthComponent;

	UPROPERTY()
		FTimerHandle GameOverTimer;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> GameOverUI;

	UPROPERTY()
		class UUserWidget* GameOverUIInstance;

	void Death(FVector DeathInstigatorLocation, FName Bone); 

	void GameOver();


public:
	// Level
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		class ULevelComponent* LevelComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		float FireRateMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		float ReloadSpeed;


public:
	// Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray<class AItem*> ItemInventory;

	UPROPERTY()
		class AItem* CurrentItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inv")
		int32 InventoryLimit;

	void Slot(int SlotNum);

	void DropItem();


public:
	// Shooting
	void Primary();

	void StopFiring();

	void Secondary();

	void Reload();


public:
	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement")
		class UPlyMovementComponent* MovementComponent;

	void Turn(float Value);

	void LookUp(float Value);

	void MoveForward(float Value);

	void MoveRight(float Value);

	void StartJump();

	void StopJump();

	void StartCrouch();

	void StopCrouch();

	void CrouchImpl(float DeltaTime);

	void Sprint();

	void StopSprinting();

protected:
	float PlyEyeHeight;
	float PlyHeight;
};