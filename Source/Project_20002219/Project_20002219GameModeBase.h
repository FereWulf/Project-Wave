#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Project_20002219GameModeBase.generated.h"

UCLASS()
class PROJECT_20002219_API AProject_20002219GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProject_20002219GameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
		class UAIWaveComponent* AIWaveComponent;

	UPROPERTY()
		class AAISpawner* Spawner;

	void StartTimer();

	void NextWave();

	void AddKills();

	void SpawnAI();

	int32 WaveKills;

	UPROPERTY()
		FTimerHandle SpawnTimer;
};
