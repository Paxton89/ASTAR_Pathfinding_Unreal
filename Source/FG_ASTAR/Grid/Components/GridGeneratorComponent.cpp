#include "FG_ASTAR/Grid/Components/GridGeneratorComponent.h"
#include "FG_ASTAR/FG_ASTARGameModeBase.h"
#include "FG_ASTAR/Grid/Tile.h"

UGridGeneratorComponent::UGridGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	GameMode = Cast<AFG_ASTARGameModeBase>(GetOwner());
}

void UGridGeneratorComponent::GenerateGrid(int GridSizeX, int GridSizeY)
{
	for (int i = 0; i < GridSizeX; ++i)
	{
		for (int j = 0; j < GridSizeY; ++j)
		{
			CreateTile(i, j);
		}
	}
	
	for (auto Tile : GameMode->AllTiles)
	{
		Tile->FindNeighbours();
	}
}

void UGridGeneratorComponent::CreateTile(int x, int y)
{
	AActor* NewActor = GetWorld()->SpawnActor(GameMode->TileBP);
	ATile* NewTile = Cast<ATile>(NewActor);
	NewTile->XPos = x;
	NewTile->YPos = y;
	auto NewTileBoundingBox = NewTile->GetComponentsBoundingBox();
	NewActor->SetActorLocation(FVector(NewTile->XPos * NewTileBoundingBox.GetSize().X, NewTile->YPos * NewTileBoundingBox.GetSize().Y,0));
	float RandNmbr = FMath::RandRange(0, 10);
	if (RandNmbr < 2)
	{
		NewTile->isIllegal = true;
	}
	NewTile->UpdateMaterial();
	NewTile->OriginLocation = NewTile->GetActorLocation();
	NewTile->TargetLocation = NewTile->GetActorLocation() + NewTile->GetActorUpVector() * 8;
	GameMode->AllTiles.Add(NewTile);
}


