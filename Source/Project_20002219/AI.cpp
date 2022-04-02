#include "AI.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "HealthComponent.h"
#include "XPOrb.h"
#include "Project_20002219GameModeBase.h"
#include "Ply.h"

AAI::AAI()
{
	PrimaryActorTick.bCanEverTick = true;

	FCollisionResponseContainer collision_response;
	collision_response.SetResponse(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	collision_response.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannels(collision_response);

	AIMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AIMesh"));
	AIMesh->SetupAttachment(RootComponent);
	AIMesh->SetCollisionProfileName(TEXT("BlockAll"));
	AIMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	AIMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
	AIMesh->bCastDynamicShadow = true;
	AIMesh->CastShadow = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->MaxHealth = 100;
	HealthComponent->Health = HealthComponent->MaxHealth;

	OrbComp = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	OrbComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OrbComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	OrbComp->SetSphereRadius(125.0f);
	OrbComp->SetupAttachment(AIMesh);
}

void AAI::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	SpawnDefaultController();

	OrbComp->OnComponentBeginOverlap.AddDynamic(this, &AAI::OnOverlapAction);
}

void AAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAIController* aiController = Cast<AAIController>(GetController());

	if (HealthComponent->Health > 0) {
		aiController->MoveToActor(Player, 10.0f, true);
	}

	FVector OrbLocation = AIMesh->GetRelativeLocation();

	float OrbFloatHeight = (FMath::Sin(FloatDisplacement) / 20);
	OrbLocation.Z += OrbFloatHeight;

	FloatDisplacement += DeltaTime;

	AIMesh->SetRelativeLocation(OrbLocation);
}

void AAI::OnOverlapAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<APly>() && HealthComponent->Health > 0) {
		APly* player = Cast<APly>(OtherActor);

		if (player->HealthComponent->Health > 0) {
			player->HealthComponent->TakeHealth(34, player);
			Destroy();

			AProject_20002219GameModeBase* GameMode = Cast<AProject_20002219GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			GameMode->AddKills(0);
		}
	}
}

void AAI::Death(FVector DeathInstigatorLocation, FName Bone) {
	FCollisionResponseContainer collision_response;
	collision_response.SetResponse(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	collision_response.SetResponse(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AIMesh->SetCollisionResponseToChannels(collision_response);

	AIMesh->SetSimulatePhysics(true);

	const FVector direction = GetActorLocation() + DeathInstigatorLocation;

	AIMesh->SetPhysicsLinearVelocity(direction, false, Bone);

	GetWorld()->SpawnActor<AXPOrb>(XPOrb, GetActorLocation(), GetActorRotation());

	AProject_20002219GameModeBase* GameMode = Cast<AProject_20002219GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->AddKills(1);

	GetWorld()->GetTimerManager().SetTimer(ClearTimer, this, &AAI::ClearRagdoll, 10.0f, false);
}

void AAI::ClearRagdoll()
{
	Destroy();
}