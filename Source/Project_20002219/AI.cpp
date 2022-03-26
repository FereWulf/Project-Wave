#include "AI.h"

#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "HealthComponent.h"
#include "XPOrb.h"

AAI::AAI()
{
	PrimaryActorTick.bCanEverTick = true;

	AIMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIMesh"));
	AIMesh->SetupAttachment(RootComponent);
	AIMesh->SetCollisionProfileName(TEXT("BlockAll"));
	AIMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	AIMesh->bCastDynamicShadow = true;
	AIMesh->CastShadow = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->MaxHealth = 100;
	HealthComponent->Health = HealthComponent->MaxHealth;
}

void AAI::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void AAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAIController* aiController = Cast<AAIController>(GetController());

	if (HealthComponent->Health > 0) {
		aiController->MoveToActor(Player, 10.0f, true);
	}
}

void AAI::Death(FVector DeathInstigatorLocation, FName Bone) {
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	AIMesh->SetSimulatePhysics(true);

	const FVector direction = GetActorLocation() + DeathInstigatorLocation;

	AIMesh->SetPhysicsLinearVelocity(direction, false, Bone);

	GetWorld()->SpawnActor<AXPOrb>(XPOrb, GetActorLocation(), GetActorRotation());

	GetWorld()->GetTimerManager().SetTimer(ClearTimer, this, &AAI::ClearRagdoll, 10.0f, false);
}

void AAI::ClearRagdoll()
{
	Destroy();
}