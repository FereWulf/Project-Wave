#include "AISpawner.h"

#include "AI.h"

void AAISpawner::Spawn()
{
    GetWorld()->SpawnActor<AAI>(AIClass, GetActorLocation() + FVector(0, 0, 90), GetActorRotation());
}