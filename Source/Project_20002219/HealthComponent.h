#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECT_20002219_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

public:

	void TakeHealth(int32 Damage, AActor* Inflictor, FVector DeathInstigatorLocation = FVector(0, 0, 0), FName Bone = "NULL");

	void AddHealth(int32 Heal);

	void Regen();

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		int32 MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		int32 Health;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
		float RegenUpgrade;

	UPROPERTY()
		FTimerHandle RegenTimer;
};
