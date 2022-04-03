#include "Ply.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Delegates/Delegate.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Blueprint/UserWidget.h"

#include "AI.h"
#include "Item.h"
#include "Project_20002219GameModeBase.h"
#include "HealthComponent.h"
#include "LevelComponent.h"
#include "StaminaComponent.h"
#include "PlyMovementComponent.h"

APly::APly(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.DoNotCreateDefaultSubobject(APly::MeshComponentName).SetDefaultSubobjectClass<UPlyMovementComponent>(APly::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = GetCapsuleComponent();

    PlyEyeHeight = 160.0f;
    PlyHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

    PlyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlyMesh"));
    PlyMesh->SetupAttachment(RootComponent);
    PlyMesh->SetCollisionObjectType(ECC_WorldStatic);
    PlyMesh->SetCollisionProfileName(TEXT("BlockAll"));
    PlyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
    PlyMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    PlyMesh->bCastDynamicShadow = true;
    PlyMesh->CastShadow = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetRelativeLocation(FVector(0.0f, 30.0f, PlyEyeHeight));
    CameraComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    CameraComponent->SetupAttachment(PlyMesh);
    CameraComponent->bUsePawnControlRotation = true;

    GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetupAttachment(CameraComponent);

    PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->MaxHealth = 100;
    HealthComponent->Health = HealthComponent->MaxHealth;

    LevelComponent = CreateDefaultSubobject<ULevelComponent>(TEXT("LevelComponent"));

    MovementComponent = Cast<UPlyMovementComponent>(GetMovementComponent());

    InventoryLimit = 1;

    // Upgrade Multipliers
    DamageMultiplier = 1.0f;
    FireRateMultiplier = 1.0f;
    ReloadSpeed = 1.0f;
}

void APly::BeginPlay()
{
    Super::BeginPlay();

    PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    CameraManager = Cast<APlayerCameraManager>(UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0));

    if (Crosshair) {
        CrosshairInstance = CreateWidget<UUserWidget>(PController, Crosshair);
        CrosshairInstance->AddToViewport();
    }

    if (UIBars) {
        UIBarsInstance = CreateWidget<UUserWidget>(PController, UIBars);
        UIBarsInstance->AddToViewport();
    }

    PController->SetInputMode(FInputModeGameOnly());
}

void APly::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Controller)
    {
        CrouchImpl(DeltaTime);

        if (CurrentItem && CurrentItem->bIsADS) {
            GunMesh->SetRelativeLocation(FMath::VInterpTo(GunMesh->GetRelativeLocation(), FVector(CurrentItem->ItemLocation.X - CurrentItem->ADSLocation.X, CurrentItem->ItemLocation.Y - CurrentItem->ADSLocation.Y, CurrentItem->ItemLocation.Z - CurrentItem->ADSLocation.Z), DeltaTime, 10.0f));
        }
        else {
            FVector camLoc;
            FRotator camRot;

            Controller->GetPlayerViewPoint(camLoc, camRot);

            FVector velocity = GetVelocity();
            velocity = camRot.UnrotateVector(velocity) * 0.005;

            GunMesh->SetRelativeLocation(FMath::VInterpTo(GunMesh->GetRelativeLocation(), FVector(30.0f, 10.0f, -10.0f) - velocity, DeltaTime, 10.0f));
        }
    }

    if (HealthComponent->Health > 0) {
        FVector camLoc;
        FRotator camRot;

        Controller->GetPlayerViewPoint(camLoc, camRot);

        const FVector startTrace = camLoc;
        const FVector direction = camRot.Vector();
        const FVector endTrace = startTrace + direction * 200;

        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(this);
        queryParams.AddIgnoredActor(CurrentItem);

        FHitResult hit(ForceInit);

        if (GetWorld()->LineTraceSingleByChannel(hit, startTrace, endTrace, ECC_Visibility, queryParams))
        {
            if (HoverActor) {
                if (HighlightItem) {
                    HighlightItem->Unfocused();
                    HighlightItem = nullptr;
                }
                HoverActor = nullptr;
            }

            HoverActor = hit.GetActor();

            if (HoverActor->IsA<AItem>()) {
                HighlightItem = Cast<AItem>(HoverActor);
                HighlightItem->Focused();
            }

            if (PController->WasInputKeyJustPressed(EKeys::E)) {
                if (HoverActor->IsA<AItem>()) {
                    Use(HoverActor);
                }
                else if (HoverActor->IsA<AAI>()) {
                    UPrimitiveComponent* component = hit.GetComponent();

                    FName bone = hit.BoneName;

                    FVector impact = hit.ImpactPoint;

                    PhysicsHandle->GrabComponentAtLocation(component, bone, impact);
                }
            }
        }
        else if (HoverActor) {
            if (HighlightItem) {
                HighlightItem->Unfocused();
                HighlightItem = nullptr;
            }
            HoverActor = nullptr;
        }

        if (PhysicsHandle->GrabbedComponent) {
            RagdollUse(camLoc, camRot);
        }
    }
}

void APly::Use(AActor* actor)
{
    if (ItemInventory.Num() == InventoryLimit)
    {
        DropItem();
    }

    CurrentItem = Cast<AItem>(actor);

    ItemInventory.Emplace(CurrentItem);
    CurrentItem->PickedUp();

    GunMesh->SetSkeletalMesh(CurrentItem->MeshComponent->SkeletalMesh);

    if (UIAmmo && ItemInventory.Num() == 1) {
        UIAmmoInstance = CreateWidget<UUserWidget>(PController, UIAmmo);
        UIAmmoInstance->AddToViewport();
    }
}

int32 APly::DisplayMag() {
    return CurrentItem->Mag;
}

void APly::RagdollUse(FVector camLoc, FRotator camRot)
{
    PhysicsHandle->SetTargetLocation(camLoc + camRot.Vector() * 200);
}

void APly::StopUse()
{
    PhysicsHandle->ReleaseComponent();
}

void APly::Slot(int SlotNum)
{
    if (LevelComponent->Levels > 0) {
        LevelComponent->ChosenUpgrade(this, SlotNum);
    }
}

void APly::DropItem()
{
    if (CurrentItem) {
        CurrentItem->bIsADS = false;

        GunMesh->SetSkeletalMesh(0);

        FVector camLoc;
        FRotator camRot;

        Controller->GetPlayerViewPoint(camLoc, camRot);

        GetWorld()->SpawnActor<AItem>(CurrentItem->GetClass(), camLoc + camRot.Vector() * 50, camRot);
        CurrentItem->Destroy();

        CurrentItem->MeshComponent->SetAllPhysicsLinearVelocity(300 * camRot.Vector(), false);

        ItemInventory.RemoveSingle(CurrentItem);
        CurrentItem = NULL;

        if (UIAmmo) {
            UIAmmoInstance->RemoveFromViewport();
        }
    }
}

void APly::Primary()
{
    if (CurrentItem) {
        CurrentItem->Primary(this);
    }
}

void APly::StopFiring() {
    if (CurrentItem) {
        CurrentItem->StopPrimary(this);
    }
}

void APly::Secondary() {
    if (CurrentItem) {
        CurrentItem->Secondary(this);
    }
}

void APly::Reload() {
    if (CurrentItem) {
        CurrentItem->Reload(this);
    }
}

void APly::Death(FVector DeathInstigatorLocation, FName Bone) {
    for (int i = 0; i < ItemInventory.Num();) {
        CurrentItem = ItemInventory[i];
        DropItem();
    }

    DisableInput(PController);

    GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

    PlyMesh->SetSimulatePhysics(true);

    const FVector direction = GetActorLocation() + DeathInstigatorLocation;

    PlyMesh->SetPhysicsLinearVelocity(direction, false, Bone);

    if (Crosshair) {
        CrosshairInstance->RemoveFromViewport();
    }

    if (UIBars) {
        UIBarsInstance->RemoveFromViewport();
    }

    if (LevelComponent->UpgradesUIInstance) {
        LevelComponent->UpgradesUIInstance->RemoveFromViewport();
    }

    GetWorld()->GetTimerManager().SetTimer(GameOverTimer, this, &APly::GameOver, 1.0f, false);
}

void APly::GameOver() {
    if (GameOverUI) {
        GameOverUIInstance = CreateWidget<UUserWidget>(PController, GameOverUI);
        GameOverUIInstance->AddToViewport();
    }

    PController->SetInputMode(FInputModeUIOnly());
    PController->bShowMouseCursor = true;

    PController->SetPause(true);
}

void APly::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up "axis" bindings.
    PlayerInputComponent->BindAxis("MoveForward", this, &APly::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APly::MoveRight);

    PlayerInputComponent->BindAxis("Turn", this, &APly::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &APly::LookUp);

    // Set up "action" bindings
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APly::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &APly::StopJump);

    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APly::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &APly::StopCrouch);

    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APly::Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APly::StopSprinting);

    PlayerInputComponent->BindAction("Primary", IE_Pressed, this, &APly::Primary);
    PlayerInputComponent->BindAction("Primary", IE_Released, this, &APly::StopFiring);

    PlayerInputComponent->BindAction("Interact", IE_Released, this, &APly::StopUse);

    PlayerInputComponent->BindAction("Secondary", IE_Pressed, this, &APly::Secondary);

    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APly::Reload);

    // Inventory
    DECLARE_DELEGATE_OneParam(FSlotNumDelegate, const int);

    PlayerInputComponent->BindAction<FSlotNumDelegate>("Slot1", IE_Pressed, this, &APly::Slot, 0);
    PlayerInputComponent->BindAction<FSlotNumDelegate>("Slot2", IE_Pressed, this, &APly::Slot, 1);
    PlayerInputComponent->BindAction<FSlotNumDelegate>("Slot3", IE_Pressed, this, &APly::Slot, 2);

    PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &APly::DropItem);
}

void APly::Turn(float Value)
{
    MovementComponent->Turn(Value);
}

void APly::LookUp(float Value)
{
    MovementComponent->LookUp(Value);
}

void APly::MoveForward(float Value)
{
    MovementComponent->MoveForward(Value);
}

void APly::MoveRight(float Value)
{
    MovementComponent->MoveRight(Value);
}

void APly::StartJump()
{
    bPressedJump = true;
}

void APly::StopJump()
{
    bPressedJump = false;
}

void APly::StartCrouch()
{
    MovementComponent->StartCrouch();
}

void APly::StopCrouch()
{
    MovementComponent->StopCrouch();
}

void APly::CrouchImpl(float DeltaTime)
{
    MovementComponent->CrouchImpl(DeltaTime, PlyEyeHeight, PlyHeight);
}

void APly::Sprint()
{
    MovementComponent->Sprint();
}

void APly::StopSprinting()
{
    MovementComponent->StopSprinting();
}