#include "LevelComponent.h"

#include "Math/UnrealMathUtility.h"

#include "Ply.h"
#include "Item.h"
#include "HealthComponent.h"
#include "PlyMovementComponent.h"

ULevelComponent::ULevelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Levels = 0;
	XP = 0;
	XPCap = 10;

	FString Arr[5] = {TEXT("Health"), TEXT("RegenTimer"), TEXT("Damage"), TEXT("Fire-Rate"), TEXT("Speed")};
	Upgrades.Append(Arr, UE_ARRAY_COUNT(Arr));
}

void ULevelComponent::AddXP()
{
	XP = XP + 1;
	if (XP == XPCap) {
		XP = 0;
		LevelUp();
	}
}

void ULevelComponent::LevelUp()
{
	if (Levels == 0) {
		DisplayUpgrades();
	}
	Levels = Levels + 1;

	XPCap = FGenericPlatformMath::RoundToInt(XPCap * 1.2);
}

void ULevelComponent::ChosenUpgrade(APly* player, int32 Slot)
{
	FString upgrade = DisplayList[Slot];
	if (upgrade == "Health") {
		player->HealthComponent->MaxHealth += 10;
		player->HealthComponent->Health += 10;

		if (player->HealthComponent->MaxHealth == 300) {
			DisplayList.Remove("Health");
		}
	} else if (upgrade == "RegenTimer") {
		player->HealthComponent->RegenUpgrade += 0.1f;

		if (player->HealthComponent->RegenUpgrade == 2.0f) {
			DisplayList.Remove("RegenTimer");
		}
	} else if (upgrade == "Damage") {
		player->DamageMultiplier += 0.1f;

		if (player->DamageMultiplier == 3.0f) {
			DisplayList.Remove("Damage");
		}
	} else if (upgrade == "Fire-Rate") {
		player->FireRateMultiplier += 0.1f;

		if (player->FireRateMultiplier == 2.0f) {
			DisplayList.Remove("Fire-Rate");
		}
	} else if (upgrade == "Speed") {
		player->MovementComponent->SpeedMultiplier += 0.1f;

		if (player->MovementComponent->SpeedMultiplier == 3.0f) {
			DisplayList.Remove("Speed");
		}
	}

	if (Levels > 0) {
		DisplayList.Empty();
		DisplayUpgrades();
	}
}

TArray<FString> ULevelComponent::DisplayUpgrades()
{
	for (int i = 0; i < 3; i++) {
		int32 val = FMath::RandRange(0, Upgrades.Num() - 1);
		DisplayList.Emplace(Upgrades[val]);
	}

	return DisplayList;
}

float ULevelComponent::DisplayXP()
{
	return (XP / float(XPCap));
}