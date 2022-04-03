#include "AIWaveComponent.h"

UAIWaveComponent::UAIWaveComponent()
{
	Wave = 1;
	AINum = 8;
	AISpawned = 0;
	Kills = 0;
	AICap = 0;
}

FText UAIWaveComponent::DisplayWave()
{
	FString display = "Wave: ";
	display.AppendInt(Wave);
	return FText::FromString(display);
}

FText UAIWaveComponent::DisplayKills()
{
	FString display = "Kills: ";
	display.AppendInt(Kills);
	return FText::FromString(display);
}