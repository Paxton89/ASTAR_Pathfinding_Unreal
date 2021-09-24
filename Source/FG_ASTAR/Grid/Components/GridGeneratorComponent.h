// Author: Johan Liljedahl
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GridGeneratorComponent.generated.h"

class AFG_ASTARGameModeBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FG_ASTAR_API UGridGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGridGeneratorComponent();
	void GenerateGrid(int GridSizeX, int GridSizeY);
	
protected:
	AFG_ASTARGameModeBase* GameMode;

	void CreateTile(int x, int y);
};
