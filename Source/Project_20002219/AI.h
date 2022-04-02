#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI.generated.h"

UCLASS()
class PROJECT_20002219_API AAI : public ACharacter
{
	GENERATED_BODY()

public:
	AAI();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* AIMesh;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* OrbComp;

	ACharacter* Player;

	float FloatDisplacement;

	virtual void Tick(float DeltaTime) override;

public:
	// Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		class UHealthComponent* HealthComponent;

	UPROPERTY()
		FTimerHandle ClearTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
		TSubclassOf<class AXPOrb> XPOrb;

	void Death(FVector DeathInstigatorLocation, FName Bone);

	void ClearRagdoll();

	UFUNCTION()
		void OnOverlapAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
