// Author: Johan Liljedahl

#include "FG_ASTAR/Grid/Tile.h"
#include "DrawDebugHelpers.h"
#include "FG_ASTAR/FG_ASTARGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionProfileName("NoCollision");
	RootComponent = Mesh;
	
	MouseOverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MouseOverMesh"));
	MouseOverMesh->SetCollisionProfileName("NoCollision");
	MouseOverMesh->SetupAttachment(RootComponent);
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetCollisionProfileName("BlockAllDynamic");
	Box->SetupAttachment(RootComponent);
}

void ATile::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AFG_ASTARGameModeBase>(GetWorld()->GetAuthGameMode());
	IgnoreList.Add(this);
	MouseOverMesh->SetMaterial(0, Mat_MouseOverLegal);
	MouseOverMesh->ToggleVisibility();
}

void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DrawDebug();
	RemoveHighlight();
	RaiseIfTargeted();
}

void ATile::FindNeighbours()
{
	if(bIsIllegal) return;
	FHitResult Hit;
	ATile* HitTile;
	//North
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	HitTile = Cast<ATile>(Hit.Actor);
	if(Hit.bBlockingHit && !HitTile->bIsIllegal)
	{
		Neighbours.Add(HitTile);
	}
	//South
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * -60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	HitTile = Cast<ATile>(Hit.Actor);
	if(Hit.bBlockingHit && !HitTile->bIsIllegal)
	{
		Neighbours.Add(HitTile);
	}
	//East
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	HitTile = Cast<ATile>(Hit.Actor);
	if(Hit.bBlockingHit && !HitTile->bIsIllegal)
	{
		Neighbours.Add(HitTile);
	}
	//West
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * -60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	HitTile = Cast<ATile>(Hit.Actor);
	if(Hit.bBlockingHit && !HitTile->bIsIllegal)
	{
		Neighbours.Add(HitTile);
	}
}

void ATile::DrawDebug()
{
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 60, FColor::MakeRandomColor(), false, 0.01, 0, 4);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * -60, FColor::MakeRandomColor(), false, 0.01, 0, 4);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 60, FColor::MakeRandomColor(), false, 0.01, 0, 4);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * -60, FColor::MakeRandomColor(), false, 0.01, 0, 4);
}

void ATile::RaiseIfTargeted()
{
	FVector CurrentLocation = GetActorLocation();
	if(bIsHighLighted)
	{
		SetActorLocation(FMath::Lerp(CurrentLocation, TargetLocation, 0.03));
	}
	else
	{
		SetActorLocation(FMath::Lerp(CurrentLocation, OriginLocation, 0.05));
	}
}

void ATile::ToggleMaterialVisible()
{
	HighlightBuffer = 0.05;
	MouseOverMesh->SetVisibility(true);
	bIsHighLighted = true;
}

void ATile::ToggleMaterialHidden()
{
	MouseOverMesh->SetVisibility(false);
	bIsHighLighted = false;
}

void ATile::UpdateMaterial()
{
	if(bIsIllegal)
	{
		Mesh->SetHiddenInGame(true);
		MouseOverMesh->SetHiddenInGame(true);
	}
	else if(bIsExpensive)
	{
		Mesh->SetMaterial(0, Mat_ExpensiveTile);
	}
}

void ATile::ResetValues()
{
	if(bIsExpensive)
	{
		G_Value = GameMode->ExpensiveTilePrice;
	}
	else
	{
		G_Value = 150;
	}
	H_Value = NULL;
    F_Value = NULL;
	Parent = nullptr;
}

void ATile::RemoveHighlight()
{
	HighlightBuffer -= GetWorld()->DeltaTimeSeconds;
	if(HighlightBuffer < 0)
	{
		ToggleMaterialHidden();
	}
}

