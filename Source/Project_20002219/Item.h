#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class APly;

UCLASS()
class PROJECT_20002219_API AItem : public AActor
{
	GENERATED_BODY()

public:
	AItem();

public:
	void PickedUp();

	virtual void Primary(APly* Player);

	virtual void StopPrimary(APly* Player);

	virtual void Secondary(APly* Player);

	virtual void Reload();

	// Interact
	void Focused();

	void Unfocused();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bIsADS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Mag;

	UPROPERTY()
		FVector ADSLocation;

	UPROPERTY()
		FVector ItemLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class USkeletalMeshComponent* MeshComponent;
};
