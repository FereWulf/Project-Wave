#include "Gun.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Blueprint/UserWidget.h"

#include "Ply.h"
#include "AI.h"
#include "HealthComponent.h"
#include "PlyMovementComponent.h"

AGun::AGun()
{
    FireRate = 50;
    Damage = 50;
    Mobility = 0.5;
    Range = 30000;
    Mag = 30;
    MaxMagSize = 30;
    ShakeFloat = 10.0f;
    KillVelocity = 3000.0f;
    bFullAuto = true;
    bIsADS = false;
}

void AGun::Primary(APly* Player)
{
    if (Mag > 0 && !GetWorldTimerManager().IsTimerActive(SpamTimer)) {
        bStopSpam = true;

        FVector camLoc;
        FRotator camRot;

        Player->Controller->GetPlayerViewPoint(camLoc, camRot);

        FVector spread;

        if (Player->GetCharacterMovement()->IsFalling()) {
            spread = FVector(FMath::FRandRange(-15000.0f, 15000.0f), FMath::FRandRange(-15000.0f, 15000.0f), 0.0f);
        }
        else {
            FVector velocity = GetVelocity();
            FRotator rotation = GetActorRotation();
            velocity = rotation.UnrotateVector(velocity);

            spread = FVector(FMath::FRandRange(-1.0f, 1.0f) * velocity.X, FMath::FRandRange(-1.0f, 1.0f) * velocity.Y, 0.0f);
        }

        const FVector startTrace = camLoc;
        const FVector direction = camRot.Vector();
        const FVector endTrace = (startTrace + direction * Range) + spread;

        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(this);
        queryParams.AddIgnoredActor(Player);

        FHitResult hit;

        if (GetWorld()->LineTraceSingleByChannel(hit, startTrace, endTrace, ECC_Visibility, queryParams))
        {
            AActor* Actor = hit.GetActor();

            FVector HitLocation = hit.Location;

            FName bone = hit.BoneName;

            UPhysicalMaterial* material = hit.PhysMaterial.Get();

            if (material) {
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, material->GetFName().ToString());
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), material->Density));
            }

            if (ImpactParticles) {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FTransform(hit.ImpactNormal.Rotation(), hit.ImpactPoint));
            }

            if (Actor->IsA<AAI>()) {
                AAI* target = Cast<AAI>(Actor);

                if (target->HealthComponent->Health > 0)
                {
                    FVector deathInstigatorLocation = camRot.Vector() * KillVelocity;

                    target->HealthComponent->TakeHealth(Damage * Player->DamageMultiplier, target, deathInstigatorLocation, bone);
                }
            }
        }

        if (MuzzleParticles) {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticles, FTransform(GetActorLocation() + camRot.Vector() * 20)); // Find tranform bone.
        }

        if (FireAnimation != NULL)
        {
            UAnimInstance* AnimInstance = NULL;
            if (AnimInstance != NULL) {
                AnimInstance->Montage_Play(FireAnimation, 1.f);
            }
        }

        Player->CameraManager->StartCameraShake(CameraShake, ShakeFloat);

        Mag = Mag - 1;

        if (bFullAuto) {
            GetWorld()->GetTimerManager().SetTimer(PrimaryTimer, Player, &APly::Primary, 5.0f / (FireRate * Player->FireRateMultiplier), false);
        }
    }
}

void AGun::StopPrimary(APly* Player)
{
    if (bStopSpam) {
        GetWorld()->GetTimerManager().SetTimer(SpamTimer, this, &AGun::AdieuTimer, 1.0f / (FireRate * Player->FireRateMultiplier), false);
        bStopSpam = false;
    }

    GetWorld()->GetTimerManager().ClearTimer(PrimaryTimer);
}

void AGun::AdieuTimer() {
    GetWorld()->GetTimerManager().ClearTimer(SpamTimer);
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
    if (Mag != MaxMagSize) {
        GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &AGun::ReloadImpl, 1.0f, false);
    }
}

void AGun::ReloadImpl() {
    Mag = MaxMagSize;

    GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
}