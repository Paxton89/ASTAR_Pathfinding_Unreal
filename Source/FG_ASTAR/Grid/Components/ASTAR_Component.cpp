// Author: Johan Liljedahl
#include "FG_ASTAR/Grid/Components/ASTAR_Component.h"
#include "FG_ASTAR/FG_ASTARGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "FG_ASTAR/Grid/Tile.h"



UASTAR_Component::UASTAR_Component()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UASTAR_Component::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AFG_ASTARGameModeBase>(GetOwner());
}

void UASTAR_Component::Calculate_G(ATile* ReceivedTile)
{
	ReceivedTile->G_Value = 1;
} // Base Cost
void UASTAR_Component::Calculate_H(ATile* ReceivedTile)
{
	float XDist = abs(EndTile->XPos - ReceivedTile->XPos);
	float YDist = abs(EndTile->YPos - ReceivedTile->YPos);
	float Manhattan = XDist + YDist;
	float Hypotenuse = (XDist * XDist) + (YDist * YDist);
	float Distance = Hypotenuse;
	ReceivedTile->H_Value = Distance;
} //Distance from end Node
void UASTAR_Component::Calculate_F(ATile* ReceivedTile)
{
	ReceivedTile->F_Value = ReceivedTile->G_Value + ReceivedTile->H_Value;
} //Base Cost + Distance from End

void UASTAR_Component::CalculatePath(ATile* Start, ATile* End)
{
	ResetTileValues();
	COLOR = FColor::MakeRandomColor();
	
	TArray<ATile*> TilesToTest;
	TArray<ATile*> ExploredTiles;
	Start->F_Value = 999;
	StartTile = Start;
	EndTile = End;
	Calculate_H(Start); // How far is start from end?
	Calculate_H(End); // Set End H to 0

	CurrentTile = Start;
	TilesToTest.Add(CurrentTile);
	while (TilesToTest.Num() > 0)
	{
		for (auto InspectedTile : CurrentTile->Neighbours)
		{
			if (!ExploredTiles.Contains(InspectedTile)) //If we have not explored this neighbour
			{
				Calculate_H(InspectedTile);
				Calculate_F(InspectedTile);
			}
			else
			{
				//We have already visited this neighbour -- Leave for-loop
				continue;
			}
			//int CostToNext = CurrentTile->G_Value + CurrentTile->F_Value + InspectedTile->F_Value;
			if (TilesToTest.Contains(InspectedTile))
			{
				/*
				if (InspectedTile->G_Value < CostToNext)
				{
					continue;
				}
				*/
				// if we have already marked this tile for testing continue;
				continue;
			}
			else
			{
				// else mark it for testing
				TilesToTest.Add(InspectedTile);
			}
			int CostToNext = CurrentTile->F_Value + InspectedTile->F_Value;
			InspectedTile->G_Value = CostToNext; //Inspected tile now gets updated cost, (CurrentF + InspectedF) -------------- //CurrentTile->G_Value +\\ removed bcus why?
			InspectedTile->Parent = CurrentTile; //Parent InspectedTile to Current
		}
		ExploredTiles.Add(CurrentTile); //Tile fully Explored
		TilesToTest.Remove(CurrentTile);
		int CheapestF = INT_MAX;
		for (auto TestedTile : TilesToTest) //Now Iterate through all tiles marked for testing
		{
			if (TestedTile->F_Value < CheapestF) // Is TestedTile cheaper than Currently cheapest F?
			{
				CurrentTile = TestedTile; //YES
				CheapestF = CurrentTile->F_Value; //This is now the cheapest price
			}
		}
		if (CurrentTile == EndTile) //Are we standing on EndTile?
		{
			DrawPath(CurrentTile); //Draw the path
			UE_LOG(LogTemp, Warning, TEXT("Path Calculated - yay!"));
			break; //DONE
		}
	}
	if (CurrentTile != EndTile) // if could not find EndTile before exhausting all availible tiles
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't calculate path"));
	}
}

void UASTAR_Component::DrawPath(ATile* Tile)
{
	if(Tile->Parent == nullptr) return;
	DrawDebugCylinder(GetWorld(), Tile->GetActorLocation(), Tile->GetActorLocation() + Tile->GetActorUpVector() * 30, 8,18, COLOR, false, 6, 0, 1);
	DrawDebugDirectionalArrow(GetWorld(), Tile->Parent->GetActorLocation() + Tile->GetActorUpVector() * 30,Tile->GetActorLocation() + Tile->GetActorUpVector() * 30, 7, COLOR, false, 10, 0,2);
	DrawPath(Tile->Parent);
}

void UASTAR_Component::ResetTileValues()
{
	for (auto Tile : GameMode->AllTiles)
	{
		Tile->ResetValues();
	}
}
