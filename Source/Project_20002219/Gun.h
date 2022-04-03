#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Gun.generated.h"

UCLASS()
class PROJECT_20002219_API AGun : public AItem
{
	GENERATED_BODY()

public:
	AGun();

public:
	virtual void Primary(APly* Player) override;

	virtual void StopPrimary(APly* Player) override;

	virtual void Secondary(APly* Player) override;

	void DepleteSprayCounter();

	virtual void Reload(APly* Player) override;

	void ReloadImpl();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UParticleSystem* MuzzleParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY()
		FTimerHandle PrimaryTimer;

	UPROPERTY()
		FTimerHandle SprayTimer;

	UPROPERTY()
		FTimerHandle ReloadTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spray")
		TArray<FVector2D> SprayCoords;

	UPROPERTY()
		int32 SprayCounter;

	UPROPERTY()
		bool bReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 MaxMagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float ShakeFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float KillVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bFullAuto;
};
