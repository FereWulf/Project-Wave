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

	ACharacter* Player;

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
};
