#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Unit_Movement_Component.generated.h"

class ATile;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_ASTAR_API UUnit_Movement_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnit_Movement_Component();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void MoveToNextTile();
	void SetTargetTile(ATile* Tile);
	
protected:
	virtual void BeginPlay() override;
	
	FVector CurrentLocation;

	TArray<ATile*> Path;
	ATile* NextTile;
	ATile* CurrentTile;
	ATile* GoalTile;

	void CreatePath(ATile* Tile);
		
};
