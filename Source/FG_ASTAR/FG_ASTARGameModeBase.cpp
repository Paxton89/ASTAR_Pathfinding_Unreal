
#include "FG_ASTARGameModeBase.h"
#include "Grid/Tile.h"
#include "Kismet/GameplayStatics.h"
#include "FG_ASTAR/Grid/Components/GridGeneratorComponent.h"
#include "FG_ASTAR/Grid/Components/ASTAR_Component.h"


AFG_ASTARGameModeBase::AFG_ASTARGameModeBase()
{
	GridGeneratorComponent = CreateDefaultSubobject<UGridGeneratorComponent>("GridGenerator");
	ASTAR_Component = CreateDefaultSubobject<UASTAR_Component>("ASTAR Component");
}

void AFG_ASTARGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->bShowMouseCursor = true;
	
	GridGeneratorComponent->GenerateGrid(GridXSize, GridYSize);
}

UASTAR_Component* AFG_ASTARGameModeBase::GetAstarComponent()
{
	return ASTAR_Component;
}