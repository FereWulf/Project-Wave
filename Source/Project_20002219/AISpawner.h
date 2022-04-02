#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawner.generated.h"

UCLASS()
class PROJECT_20002219_API AAISpawner : public AActor
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf<class AAI> AIClass;

	void Spawn();
};
