// Author: Johan Liljedahl

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FG_ASTARGameModeBase.generated.h"

class ATile;
class ACameraPawn;
UCLASS()
class FG_ASTAR_API AFG_ASTARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
	APlayerController* PlayerController;
	ATile* HighlightedTile;
	ATile* CurrentTile;
	ACameraPawn* CameraPawn;
	void CalculatePath(ATile* Start, ATile* End);
	
	protected:
	AFG_ASTARGameModeBase();
	virtual void BeginPlay() override;
	void GenerateGrid();
	void DrawPath(ATile* Tile);
	void Calculate_G(ATile* CurrentTile);
	void Calculate_H(ATile* CurrentTile);
	void Calculate_F(ATile* CurrentTile);

	
	UPROPERTY(EditAnywhere)
	int GridXSize = 3;
	UPROPERTY(EditAnywhere)
	int GridYSize = 3;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> TileBP;

	TArray<ATile*> AllTiles;
	ATile* StartTile;
	ATile* EndTile;
	ATile* Parent;
	
	FColor COLOR;
};
