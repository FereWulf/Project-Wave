#include "StaminaComponent.h"

#include "Ply.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStaminaComponent::TakeStamina()
{
    Stamina = FMath::Clamp(Stamina - 1, 0, MaxStamina);
}

void UStaminaComponent::AddStamina()
{
    Stamina = FMath::Clamp(Stamina + 1, 0, MaxStamina);
}

float UStaminaComponent::GetStamina() {
    return (Stamina / float(MaxStamina));
}
