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

	void AdieuTimer();

	virtual void Reload() override;

	void ReloadImpl();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UParticleSystem* MuzzleParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		class UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY()
		FTimerHandle PrimaryTimer;

	UPROPERTY()
		FTimerHandle SpamTimer;

	UPROPERTY()
		FTimerHandle ReloadTimer;

	UPROPERTY()
		bool bStopSpam;

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
