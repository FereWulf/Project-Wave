#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIWaveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_20002219_API UAIWaveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAIWaveComponent();

public:
	int32 Wave;

	int32 AINum;

	int32 AISpawned;
	
	int32 Kills;

	UFUNCTION(BlueprintPure, Category = "AI")
		FText DisplayWave();

	UFUNCTION(BlueprintPure, Category = "AI")
		FText DisplayKills();
};
