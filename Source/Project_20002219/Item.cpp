#include "Item.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetMobility(EComponentMobility::Movable);
	CapsuleComponent->WakeRigidBody();
	CapsuleComponent->SetSimulatePhysics(true);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CapsuleComponent);
	MeshComponent->SetCustomDepthStencilValue(255);

	RootComponent = CapsuleComponent;
}

void AItem::PickedUp()
{
	SetActorHiddenInGame(true);

	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	ItemLocation = GetActorLocation();
	ADSLocation = MeshComponent->GetSocketLocation(TEXT("ADSSocket"));
}

void AItem::Primary(APly* Player)
{

}

void AItem::StopPrimary(APly* Player)
{

}

void AItem::Secondary(APly* Player)
{

}

void AItem::Reload()
{

}

void AItem::Focused() {
	MeshComponent->SetRenderCustomDepth(true);
}

void AItem::Unfocused() {
	MeshComponent->SetRenderCustomDepth(false);
}