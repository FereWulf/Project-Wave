#include "XPOrb.h"

#include "Components/SphereComponent.h" 

#include "Ply.h"
#include "LevelComponent.h"

AXPOrb::AXPOrb()
{
	PrimaryActorTick.bCanEverTick = true;

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	OrbMesh->SetCollisionProfileName(TEXT("OverlapAll"));
	OrbMesh->SetCustomDepthStencilValue(255);

	RootComponent = OrbMesh;

	OrbComp = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	OrbComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OrbComp->SetSphereRadius(400.0f);
	OrbComp->SetupAttachment(RootComponent);

	OrbComp->OnComponentBeginOverlap.AddDynamic(this, &AXPOrb::OrbBeginOverlap);
}

void AXPOrb::BeginPlay()
{
	Super::BeginPlay();
}

void AXPOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector OrbLocation = GetActorLocation();

	float OrbFloatHeight = (FMath::Sin(OrbDisplacement) / 20);
	OrbLocation.Z += OrbFloatHeight;

	OrbDisplacement += DeltaTime;

	SetActorLocation(OrbLocation);
}

void AXPOrb::OrbBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (OtherActor->IsA<APly>()) {
		APly* player = Cast<APly>(OtherActor);
		player->LevelComponent->AddXP();
		Destroy();
	}
}