// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XPOrb.generated.h"

UCLASS()
class PROJECT_20002219_API AXPOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXPOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float OrbDisplacement;

	UFUNCTION()
		void OrbBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* OrbComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* OrbMesh;
};
