// Author: Johan Liljedahl
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
	NewTile->OriginLocation = NewTile->GetActorLocation();
	NewTile->TargetLocation = NewTile->GetActorLocation() + NewTile->GetActorUpVector() * 8;
	
	float RandNmbr = FMath::RandRange(0, 10);
	if (RandNmbr < 2)
	{
		NewTile->bIsIllegal = true;
	}
	else if(RandNmbr < 3 && !NewTile->bIsIllegal && GameMode->bMakeSomeTilesExpensive)
	{
		NewTile->OriginLocation = NewTile->GetActorLocation() + NewTile->GetActorUpVector() * 16;
		NewTile->TargetLocation = NewTile->GetActorLocation() + NewTile->GetActorUpVector() * 20;
		NewTile->bIsExpensive = true;
	}
	NewTile->ResetValues();
	NewTile->UpdateMaterial();
	GameMode->AllTiles.Add(NewTile);
}


