#include "PlyMovementComponent.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

#include "StaminaComponent.h"
#include "Ply.h"
#include "Item.h"

UPlyMovementComponent::UPlyMovementComponent()
{
    Player = Cast<APly>(GetOwner());

    Sensitivity = 1;

    SprintSpeed = 600.0f;
    CrouchSpeed = 200.0f;
    WalkSpeed = 300.0f;
    JumpSpeed = 400.0f;
    MaxWalkSpeed = WalkSpeed;
    JumpZVelocity = JumpSpeed;

    NavAgentProps.bCanCrouch = true;

    AirControl = 1.0f;
    GroundFriction = 2.0f;
    BrakingFriction = 0.5f;

    StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
    StaminaComponent->MaxStamina = 100;
    StaminaComponent->Stamina = StaminaComponent->MaxStamina;
}

void UPlyMovementComponent::Turn(float Value)
{
    Player->AddControllerYawInput(Sensitivity * Value / 6); // * GetWorld()->GetDeltaSeconds() is like mouse acceleration/smoothing.

    if (IsFalling()) {

        bool bKeyHeldA = Player->PController->IsInputKeyDown(EKeys::A);
        bool bKeyHeldD = Player->PController->IsInputKeyDown(EKeys::D);

        bool bKeyHeldS = Player->PController->IsInputKeyDown(EKeys::S);

        FVector camLoc;
        FRotator camRot;

        Player->Controller->GetPlayerViewPoint(camLoc, camRot);

        const FVector direction = camRot.Vector();
        float value = fabs(Value);

        if (value > 0.75) {
            StrafeThreshhold = true;
        }

        if (bKeyHeldS) {
            value = -value;
        }

        if (Value < 0 && StrafeThreshhold && bKeyHeldA) {
            Player->AddMovementInput(direction, value);
        }
        else if (Value > 0 && StrafeThreshhold && bKeyHeldD) {
            Player->AddMovementInput(direction, value);
        }
    }
    else {
        StrafeThreshhold = false;
    }
}

void UPlyMovementComponent::LookUp(float Value)
{
    Player->AddControllerPitchInput(Sensitivity * Value / 6);
}

void UPlyMovementComponent::MoveForward(float Value)
{
    const FRotator rotation = Player->Controller->GetControlRotation();
    const FRotator yawRotation(0, rotation.Yaw, 0);

    FVector direction = FRotationMatrix(yawRotation).GetScaledAxis(EAxis::X);

    if (IsFalling()) {
        Value = Value * 0.2;
    }
    Player->AddMovementInput(direction, Value);
}

void UPlyMovementComponent::MoveRight(float Value)
{
    FVector direction = FRotationMatrix(Player->Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);

    if (IsFalling()) {
        Value = Value * 0.2;
    }
    Player->AddMovementInput(direction, Value);
}

void UPlyMovementComponent::StartCrouch()
{
    bIsCrouching = true;
}

void UPlyMovementComponent::StopCrouch()
{
    if (!uncrouchBlocked) {
        bIsCrouching = false;
    }

    if (Player->CurrentItem && Player->CurrentItem->bIsADS) {
        MaxWalkSpeed = CrouchSpeed;
    }
    else {
        MaxWalkSpeed = WalkSpeed;
    }
}

void UPlyMovementComponent::CrouchImpl(float DeltaTime, float EyeHeight, float Height)
{
    FVector camLoc;
    FRotator camRot;

    Player->Controller->GetPlayerViewPoint(camLoc, camRot);

    const FVector startTrace = GetActorLocation();
    float traceHeight = 88.0f;
    if (bIsCrouching) {
        traceHeight = 176.0f - Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    }
    const FVector endTrace = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + traceHeight);

    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(Player);
    queryParams.AddIgnoredActor(Player->CurrentItem);

    FHitResult hit(ForceInit);

    if (GetWorld()->LineTraceSingleByChannel(hit, startTrace, endTrace, ECC_Visibility, queryParams))
    {
        uncrouchBlocked = true;
    }
    else if (uncrouchBlocked && bIsCrouching && !Player->PController->IsInputKeyDown(EKeys::LeftControl)) {
        uncrouchBlocked = false;
        StopCrouch();
    }

    const float TargetBEH = bIsCrouching ? EyeHeight/1.25 : EyeHeight;

    const float TargetCapsuleSize = bIsCrouching ? 56.0f : Height;

    Player->BaseEyeHeight = FMath::FInterpTo(Player->BaseEyeHeight, TargetBEH, DeltaTime, 10.0f);
    Player->CameraComponent->SetRelativeLocation(FVector(0.0f, 30.0f, Player->BaseEyeHeight));

    Player->GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::FInterpTo(Player->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), TargetCapsuleSize, DeltaTime, 5.0f), true);

    if (bIsCrouching) {
        MaxWalkSpeed = FMath::FInterpTo(MaxWalkSpeed, CrouchSpeed, DeltaTime, 2.5f);
    }
}

void UPlyMovementComponent::Sprint()
{
    if (!bIsCrouching) {
        if (Player->CurrentItem && Player->CurrentItem->bIsADS) {
            MaxWalkSpeed = CrouchSpeed;
        }
        else {
            if (StaminaComponent->Stamina > 17) {
                MaxWalkSpeed = SprintSpeed;

                GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &UPlyMovementComponent::LoseStamina, 0.1f, true);
            }
        }
    }
}

void UPlyMovementComponent::LoseStamina()
{
    if (Velocity.Size() > 0.0f) {
        StaminaComponent->TakeStamina();

        if (StaminaComponent->Stamina == 0) {
            StopSprinting();
        }
    }
}

void UPlyMovementComponent::StopSprinting()
{
    if (!bIsCrouching) {
        if (Player->CurrentItem && Player->CurrentItem->bIsADS) {
            MaxWalkSpeed = CrouchSpeed;
        }
        else {
            MaxWalkSpeed = WalkSpeed;
        }

        GetWorld()->GetTimerManager().SetTimer(SprintTimer, this, &UPlyMovementComponent::GainStamina, 0.1f, true, 2.0f);
    }
}

void UPlyMovementComponent::GainStamina()
{
    StaminaComponent->AddStamina();
}