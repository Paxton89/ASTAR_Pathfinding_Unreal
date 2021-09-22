#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseUnit.generated.h"

class UStaticMeshComponent;
class UUnit_Movement_Component;
class UBoxComponent;
UCLASS()
class FG_ASTAR_API ABaseUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseUnit();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UBoxComponent* Box;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UUnit_Movement_Component* MovementComponent;
};
