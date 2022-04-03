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

	void DisplayUpgrades();

	UFUNCTION(BlueprintPure, Category = "Level")
		FString DisplayFirstIndex();

	UFUNCTION(BlueprintPure, Category = "Level")
		FString DisplaySecondIndex();

	UFUNCTION(BlueprintPure, Category = "Level")
		FString DisplayThirdIndex();

	UFUNCTION(BlueprintPure, Category = "Level")
		float DisplayXP();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> UpgradesUI;

	UPROPERTY()
		class UUserWidget* UpgradesUIInstance;

	int32 Levels;

	int32 XP;

	int32 XPCap;

	float SpeedMultiplier;

	TArray<FString> Upgrades;

	TArray<FString> DisplayList;
};
