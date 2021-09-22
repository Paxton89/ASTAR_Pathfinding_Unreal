
#include "FG_ASTAR/Units/Components/Unit_Movement_Component.h"

#include "FG_ASTAR/Grid/Tile.h"


UUnit_Movement_Component::UUnit_Movement_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUnit_Movement_Component::BeginPlay()
{
	Super::BeginPlay();
}

void UUnit_Movement_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		UE_LOG(LogTemp, Log, TEXT("AAAAAAAAAAAAAAA"));
		MoveToNextTile();	

}

void UUnit_Movement_Component::CreatePath(ATile* Tile)
{
	if(Tile->Parent == nullptr)
	{
		CurrentTile = Tile;
		UE_LOG(LogTemp, Log, TEXT("Found first tile - %i, %i"), Tile->XPos, Tile->YPos);
	}
	else if(Tile != nullptr)
	{
		Path.Add(Tile);
		CreatePath(Tile->Parent);
		UE_LOG(LogTemp, Log, TEXT("Trying to find first tile - Backing up"))
	}
}

void UUnit_Movement_Component::SetTargetTile(ATile* Tile)
{
	GoalTile = Tile;
	CreatePath(GoalTile);
}

void UUnit_Movement_Component::MoveToNextTile()
{
	if(CurrentLocation != NextTile->TargetLocation)
	{
		CurrentLocation = GetOwner()->GetActorLocation();
		FMath::Lerp(CurrentLocation, NextTile->TargetLocation, 0.1f);	
	}
	else
	{
		if(Path.Num() > 0)
		{
			NextTile = Path.Pop();	
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Actor says path array is empty"))
		}
	}
}


