// Author: Johan Liljedahl

#pragma once

#include "CoreMinimal.h"
#include "CameraPawn.generated.h"

class ATile;
class AFG_ASTARGameModeBase;
class UCameraComponent;
UCLASS()
class FG_ASTAR_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ACameraPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void EmptyPairedList();
	
protected:
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LeftClick();

	UPROPERTY(EditAnywhere)
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere)
	UCameraComponent* MainCamera;

	float MoveX;
	float MoveY;

	TArray<AActor*> IgnoreList;
	TArray<ATile*> PairedTiles;
	AFG_ASTARGameModeBase* GameMode;
	ATile* TargetedTile;
	ATile* StartTile;
	ATile* EndTile;
};
