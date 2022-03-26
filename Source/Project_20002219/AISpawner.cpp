#include "AISpawner.h"

#include "AI.h"

void AAISpawner::Spawn()
{
    FRotator AIRotation(0, 0, 0);

    FActorSpawnParameters AISpawnInfo;

    AAI* AISpawn = GetWorld()->SpawnActor<AAI>(AAI::StaticClass(), GetActorLocation(), AIRotation, AISpawnInfo);
}