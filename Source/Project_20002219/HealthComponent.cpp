#include "HealthComponent.h"

#include "Math/UnrealMathUtility.h"

#include "Ply.h"
#include "AI.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	RegenUpgrade = 0;
}

void UHealthComponent::TakeHealth(int32 Damage, AActor* Inflictor, FVector DeathInstigatorLocation, FName Bone)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);

	if (Health == 0) {
		if (Inflictor->IsA<APly>()) {
			APly* player = Cast<APly>(Inflictor);
			player->Death(DeathInstigatorLocation, Bone);
		} else if (Inflictor->IsA<AAI>()) {
			AAI* ai = Cast<AAI>(Inflictor);
			ai->Death(DeathInstigatorLocation, Bone);
		}
	} else if (Inflictor->IsA<APly>()) {
		GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &UHealthComponent::Regen, 3.0f - RegenUpgrade, false);
	}
}

void UHealthComponent::Regen()
{
	AddHealth(1);

	if (Health < MaxHealth) {
		GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &UHealthComponent::Regen, 0.1f, false);
	}
}

void UHealthComponent::AddHealth(int32 Heal)
{
	Health = FMath::Clamp(Health + Heal, 0, MaxHealth);
}

float UHealthComponent::GetHealth()
{
	return (Health / float(MaxHealth));
}