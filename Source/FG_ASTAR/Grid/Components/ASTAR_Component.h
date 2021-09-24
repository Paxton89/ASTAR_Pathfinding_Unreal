// Author: Johan Liljedahl
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASTAR_Component.generated.h"

class AFG_ASTARGameModeBase;
class ATile;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_ASTAR_API UASTAR_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UASTAR_Component();

	void CalculatePath(ATile* Start, ATile* End);
protected:
	virtual void BeginPlay() override;
	
	AFG_ASTARGameModeBase* GameMode;
	ATile* CurrentTile;
	ATile* StartTile;
	ATile* EndTile;
	
	FColor COLOR;

	void ResetTileValues();
	void DrawPath(ATile* Tile);
	void Update_G(ATile* ReceivedTile);
	void Update_H(ATile* ReceivedTile);
	void Update_F(ATile* ReceivedTile);
};
