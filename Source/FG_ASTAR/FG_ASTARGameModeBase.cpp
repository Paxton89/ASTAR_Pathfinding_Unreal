// Copyright Epic Games, Inc. All Rights Reserved.


#include "FG_ASTARGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "FG_ASTAR/Camera/CameraPawn.h"
#include "Grid/Tile.h"
#include "Kismet/GameplayStatics.h"

AFG_ASTARGameModeBase::AFG_ASTARGameModeBase()
{
}

void AFG_ASTARGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
	CameraPawn = Cast<ACameraPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraPawn::StaticClass()));
	GenerateGrid();
	for (auto Tile : AllTiles)
	{
		Tile->FindNeighbours();
	}
}

void AFG_ASTARGameModeBase::GenerateGrid()
{
	for (int i = 0; i < GridXSize; ++i)
	{
		for (int j = 0; j < GridYSize; ++j)
		{
			AActor* NewActor = GetWorld()->SpawnActor(TileBP);
			ATile* NewTile = Cast<ATile>(NewActor);
			NewTile->XPos = i;
			NewTile->YPos = j;
			auto NewTileBoundingBox = NewTile->GetComponentsBoundingBox();
			NewActor->SetActorLocation(FVector(i * NewTileBoundingBox.GetSize().X, j * NewTileBoundingBox.GetSize().Y, 0));
			float RandNmbr = FMath::RandRange(0, 10);
			if(RandNmbr < 2)
			{
				NewTile->isIllegal = true;
			}
			NewTile->UpdateMaterial();
			NewTile->OriginLocation = NewTile->GetActorLocation();
			NewTile->TargetLocation = NewTile->GetActorLocation() + NewTile->GetActorUpVector() * 8;
			AllTiles.Add(NewTile);
			//UE_LOG(LogTemp, Warning, TEXT("Tile: %i,%i"),NewTile->XPos, NewTile->YPos);
		}
	}
}

void AFG_ASTARGameModeBase::Calculate_G(ATile* RecivedTile) //Distance from starting node
{
	RecivedTile->G_Value = 1;
}

void AFG_ASTARGameModeBase::Calculate_H(ATile* RecivedTile) // Distance from end node
{
	float XDist = abs(EndTile->XPos - RecivedTile->XPos);
	float YDist = abs(EndTile->YPos - RecivedTile->YPos);
	float Manhattan = XDist + YDist;
	float Hypotenuse = (XDist * XDist) + (YDist * YDist);
	float Distance = Hypotenuse;
	RecivedTile->H_Value = Distance;
}

void AFG_ASTARGameModeBase::Calculate_F(ATile* RecivedTile)
{
	RecivedTile->F_Value = RecivedTile->G_Value + RecivedTile->H_Value;
}

void AFG_ASTARGameModeBase::CalculatePath(ATile* Start, ATile* End)
{
	//Init :)
	for (auto Tile : AllTiles)
    {
    	Tile->ResetValues();
    }
	TArray<ATile*> OpenList;
	TArray<ATile*> ClosedList;
	//TArray<ATile*> Path;
	Start->F_Value = 100;
	StartTile = Start;
	EndTile = End;
	Calculate_H(Start); // How far is start from end?
	Calculate_H(End); // Set End H to 0

	CurrentTile = Start;
	OpenList.Add(CurrentTile);
	while (OpenList.Num() > 0)
	{
		for (auto NextTile : CurrentTile->Neighbours)
		{
			if (!ClosedList.Contains(NextTile))
			{
				Calculate_H(NextTile);
				Calculate_F(NextTile);
			} 
			int CostToNext = CurrentTile->G_Value + CurrentTile->F_Value + NextTile->F_Value;
			if (OpenList.Contains(NextTile))
			{
				if (NextTile->G_Value < CostToNext)
				{
					continue;
				}
			}
			else
			{
				OpenList.Add(NextTile);		
			}
			NextTile->G_Value = CostToNext;
			NextTile->Parent = CurrentTile;
		}
		UE_LOG(LogTemp, Log, TEXT("exited foreach loop"));
		ClosedList.Add(CurrentTile);
		OpenList.Remove(CurrentTile);
		int GigaF = 9999999;
		for (auto OpenTile : OpenList)
		{
			if(OpenTile->F_Value < GigaF)
			{
				CurrentTile = OpenTile;
				GigaF = CurrentTile->F_Value;
			}
		}
		//Path.AddUnique(CurrentTile);
		if(CurrentTile == EndTile)
		{
			DrawPath(ClosedList);
			UE_LOG(LogTemp, Warning, TEXT("Path Calculated"));
			break;
		}
	}
	if (CurrentTile != EndTile)
	{
		UE_LOG(LogTemp, Log, TEXT("Couldn't calculate path"));
	}
}

void AFG_ASTARGameModeBase::DrawPath(TArray<ATile*> List)
{
	auto COLOR = FColor::MakeRandomColor();
	for (auto Tile : List)
	{
		DrawDebugCylinder(GetWorld(), Tile->GetActorLocation(), Tile->GetActorLocation() + Tile->GetActorUpVector() * 30, 8, 18, COLOR, false, 6, 0, 1);
	}
}
