#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelComponent.generated.h"

class APly;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_20002219_API ULevelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	ULevelComponent();

public:	
	void LevelUp();

	void ChosenUpgrade(APly* player, int32 Slot);

	void AddXP();

	UFUNCTION(BlueprintPure, Category = "Level")
		TArray<FString> DisplayUpgrades();

	UFUNCTION(BlueprintPure, Category = "Level")
		float DisplayXP();

	int32 Levels;

	int32 XP;

	int32 XPCap;

	float FireRateAdjust;

	float DamageAdjust;

	TArray<FString> Upgrades;

	TArray<FString> DisplayList;
};
