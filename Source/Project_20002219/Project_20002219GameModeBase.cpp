#include "Project_20002219GameModeBase.h"

#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "AIWaveComponent.h"
#include "AISpawner.h"

AProject_20002219GameModeBase::AProject_20002219GameModeBase()
{
    AIWaveComponent = CreateDefaultSubobject<UAIWaveComponent>(TEXT("AIWaveComponent"));
}

void AProject_20002219GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    StartTimer();
}

void AProject_20002219GameModeBase::StartTimer()
{
    //GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AProject_20002219GameModeBase::SpawnAI, 2.0f, true, 5.0f);
}

void AProject_20002219GameModeBase::NextWave()
{
    AIWaveComponent->Wave = AIWaveComponent->Wave + 1;

    AIWaveComponent->AINum = FMath::RoundToInt(AIWaveComponent->AINum + (AIWaveComponent->AINum * 1.5));

    AIWaveComponent->AISpawned = 0;

    WaveKills = 0;

    StartTimer();
}

void AProject_20002219GameModeBase::AddKills()
{
    AIWaveComponent->Kills += 1;
    WaveKills += 1;

    if (WaveKills >= AIWaveComponent->AINum) {
        NextWave();
    }
}

void AProject_20002219GameModeBase::SpawnAI()
{
    if (AIWaveComponent->AISpawned < AIWaveComponent->AINum) {
        AIWaveComponent->AISpawned += 1;

        Spawner->Spawn();
    }
    else {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
    }
}