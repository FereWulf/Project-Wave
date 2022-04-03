#include "LevelComponent.h"

#include "Math/UnrealMathUtility.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "Ply.h"
#include "Item.h"
#include "HealthComponent.h"
#include "PlyMovementComponent.h"

ULevelComponent::ULevelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Levels = 0;
	XP = 0;
	XPCap = 4;

	SpeedMultiplier = 1.0f;

	FString Arr[5] = {TEXT("Health"), TEXT("RegenTimer"), TEXT("Damage"), TEXT("Fire-Rate"), TEXT("Speed")};
	Upgrades.Append(Arr, UE_ARRAY_COUNT(Arr));
}

void ULevelComponent::AddXP()
{
	XP += 1;
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
	Levels += 1;

	XPCap = FGenericPlatformMath::RoundToInt(XPCap * 1.2);
}

void ULevelComponent::ChosenUpgrade(APly* player, int32 Slot)
{
	Levels = FMath::Clamp(Levels -= 1, 0, Levels);

	FString upgrade = DisplayList[Slot];
	if (upgrade == "Health") {
		player->HealthComponent->MaxHealth += 25;
		player->HealthComponent->Health += 25;

		if (player->HealthComponent->MaxHealth == 300) {
			DisplayList.Remove("Health");
		}
	} else if (upgrade == "RegenTimer") {
		player->HealthComponent->RegenUpgrade += 0.125f;

		if (player->HealthComponent->RegenUpgrade == 2.0f) {
			DisplayList.Remove("RegenTimer");
		}
	} else if (upgrade == "Damage") {
		player->DamageMultiplier += 0.125f;

		if (player->DamageMultiplier == 2.0f) {
			DisplayList.Remove("Damage");
		}
	} else if (upgrade == "Fire-Rate") {
		player->FireRateMultiplier += 0.125f;

		if (player->FireRateMultiplier == 2.0f) {
			DisplayList.Remove("Fire-Rate");
		}
	} else if (upgrade == "Speed") {
		SpeedMultiplier += 0.125f;

		player->MovementComponent->SprintSpeed *= 1.125f;
		player->MovementComponent->CrouchSpeed *= 1.125f;
		player->MovementComponent->WalkSpeed *= 1.125f;
		player->MovementComponent->MaxWalkSpeed = player->MovementComponent->WalkSpeed;

		if (SpeedMultiplier == 2.0f) {
			DisplayList.Remove("Speed");
		}
	}

	if (UpgradesUIInstance) {
		UpgradesUIInstance->RemoveFromViewport();
	}

	if (Levels > 0) {
		DisplayUpgrades();
	}
}

void ULevelComponent::DisplayUpgrades()
{
	DisplayList.Empty();

	for (int i = 0; i < 3; i++) {
		int32 val = FMath::RandRange(0, Upgrades.Num() - 1);
		DisplayList.Emplace(Upgrades[val]);
	}

	DisplayFirstIndex();
	DisplaySecondIndex();
	DisplayThirdIndex();

	if (UpgradesUI) {
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		UpgradesUIInstance = CreateWidget<UUserWidget>(playerController, UpgradesUI);
		UpgradesUIInstance->AddToViewport();
	}
}

FString ULevelComponent::DisplayFirstIndex()
{
	return DisplayList[0];
}

FString ULevelComponent::DisplaySecondIndex()
{
	return DisplayList[1];
}

FString ULevelComponent::DisplayThirdIndex()
{
	return DisplayList[2];
}

float ULevelComponent::DisplayXP()
{
	return (XP / float(XPCap));
}