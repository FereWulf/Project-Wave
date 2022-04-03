#include "Gun.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Blueprint/UserWidget.h"
#include "EngineUtils.h"

#include "DrawDebugHelpers.h"

#include "Ply.h"
#include "AI.h"
#include "XPOrb.h"
#include "HealthComponent.h"
#include "PlyMovementComponent.h"

AGun::AGun()
{
    FireRate = 50;
    Damage = 50;
    Range = 30000;
    Mag = 30;
    MaxMagSize = 30;
    ShakeFloat = 10.0f;
    KillVelocity = 3000.0f;
    bFullAuto = true;
    bIsADS = false;

    SprayCounter = 0;
}

void AGun::Primary(APly* Player)
{
    if (Mag > 0 && !bReloading) {
        APlayerController* playerController = GetWorld()->GetFirstPlayerController();

        int32 screenX;
        int32 screenY;
        playerController->GetViewportSize(screenX, screenY);

        screenX /= 2;
        screenY /= 2;

        FVector2D spread = SprayCoords[SprayCounter];
        float spreadAmount = 1.0f;

        if (Player->GetCharacterMovement()->IsFalling()) {
            spread = FVector2D(FMath::FRandRange(-15000.0f, 15000.0f), FMath::FRandRange(-15000.0f, 15000.0f));
            spreadAmount = 5.0f;
        }
        else {
            float speed = Player->GetVelocity().Size() / 300.0f;

            if (speed > 1) {
                spreadAmount = speed;
            } else {
                spreadAmount = 1.0f;
            }
        }

        FVector direction;
        FVector startTrace;
        playerController->DeprojectScreenPositionToWorld(screenX + spread.X, screenY + spread.Y, startTrace, direction);

        FVector endTrace = (startTrace + direction * Range) * spreadAmount;

        DrawDebugLine(GetWorld(), startTrace + direction * 50, endTrace, FColor(50, 50, 50, 255), false, 1.0f, 0, 1);

        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(this);
        queryParams.AddIgnoredActor(Player);
        
        for (TActorIterator<AXPOrb> ActorItr(GetWorld()); ActorItr; ++ActorItr)
        {
            queryParams.AddIgnoredActor(*ActorItr);
        }

        FHitResult hit;

        if (GetWorld()->LineTraceSingleByChannel(hit, startTrace, endTrace, ECC_Visibility, queryParams))
        {
            AActor* Actor = hit.GetActor();

            FVector HitLocation = hit.Location;

            FName bone = hit.BoneName;

            if (ImpactParticles) {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(hit.ImpactNormal.Rotation(), hit.ImpactPoint));
            }

            if (Actor->IsA<AAI>()) {
                AAI* target = Cast<AAI>(Actor);

                if (target->HealthComponent->Health > 0)
                {
                    FVector deathInstigatorLocation = direction * KillVelocity;

                    target->HealthComponent->TakeHealth(Damage * Player->DamageMultiplier, target, deathInstigatorLocation, bone);
                }
            }
        }

        if (MuzzleParticles) {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticles, FTransform(GetActorLocation() + direction * 20)); // Find tranform bone.
        }

        Player->CameraManager->StartCameraShake(CameraShake, ShakeFloat);

        Mag -= 1;

        SprayCounter += 1;

        if (bFullAuto) {
            GetWorld()->GetTimerManager().SetTimer(PrimaryTimer, Player, &APly::Primary, 5.0f / (FireRate * Player->FireRateMultiplier), false);
        }
    }
}

void AGun::StopPrimary(APly* Player)
{
    GetWorld()->GetTimerManager().SetTimer(SprayTimer, this, &AGun::DepleteSprayCounter, 0.03f, true);
    GetWorld()->GetTimerManager().ClearTimer(PrimaryTimer);
}

void AGun::DepleteSprayCounter() { 
    if (SprayCounter == 0) {
        GetWorld()->GetTimerManager().ClearTimer(SprayTimer);
    }
    else {
        SprayCounter -= 1;
    }
}

void AGun::Secondary(APly* Player)
{
    if (bIsADS) {
        bIsADS = false;

        Player->MovementComponent->MaxWalkSpeed = Player->MovementComponent->WalkSpeed;

        Player->CameraComponent->SetFieldOfView(90.0f);

        Player->CrosshairInstance->AddToViewport();
    }
    else {
        bIsADS = true;

        Player->CameraComponent->SetFieldOfView(50.0f);

        Player->CrosshairInstance->RemoveFromViewport();
    }
}

void AGun::Reload() {
    if (Mag != MaxMagSize && bReloading == false) {
        bReloading = true;
        GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AGun::ReloadImpl, 1.0f, false);
    }
}

void AGun::ReloadImpl() {
    Mag = MaxMagSize;

    SprayCounter = 0;

    bReloading = false;
    GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
}