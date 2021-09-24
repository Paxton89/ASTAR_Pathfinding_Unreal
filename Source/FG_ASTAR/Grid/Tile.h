// Author: Johan Liljedahl

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class AFG_ASTARGameModeBase;
UCLASS()
class FG_ASTAR_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATile();
	
	bool bIsIllegal = false;
	bool bIsExpensive = false;
	int XPos;
	int YPos;

	double G_Value = 150; //Base Tile Cost
	double H_Value; // Distance From End
	double F_Value; // Distance from End + Base Tile Cost

	FVector OriginLocation;
	FVector TargetLocation;
	
	TArray<ATile*> Neighbours;
	ATile* Parent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MouseOverMesh;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Mat_MouseOverIllegal;
	
	void FindNeighbours();
	void ToggleMaterialVisible();
	void ToggleMaterialHidden();
	void UpdateMaterial();
	void ResetValues();
protected:
	float HighlightBuffer = 0.05;
	
	bool bIsHighLighted = false;
	
	TArray<AActor*> IgnoreList;
	AFG_ASTARGameModeBase* GameMode;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Mat_MouseOverLegal;
	UPROPERTY(EditAnywhere)
	UMaterialInstance* Mat_ExpensiveTile;
	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void RemoveHighlight();
	void DrawDebug();
	void RaiseIfTargeted();
};
