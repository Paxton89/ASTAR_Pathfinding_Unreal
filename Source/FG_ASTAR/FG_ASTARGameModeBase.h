// Author: Johan Liljedahl

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FG_ASTARGameModeBase.generated.h"

class ATile;
class ACameraPawn;
class UGridGeneratorComponent;
class UASTAR_Component;
UCLASS()
class FG_ASTAR_API AFG_ASTARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATile> TileBP;
	
	APlayerController* PlayerController;
	
	TArray<ATile*> AllTiles;
	
	UASTAR_Component* GetAstarComponent();
	
	protected:
	AFG_ASTARGameModeBase();
	virtual void BeginPlay() override;

	UGridGeneratorComponent* GridGeneratorComponent;
	UASTAR_Component* ASTAR_Component;
	
	UPROPERTY(EditAnywhere)
	int GridXSize = 3;
	UPROPERTY(EditAnywhere)
	int GridYSize = 3;
};
