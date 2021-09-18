//Author: Johan Liljedahl

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
	FHitResult Hit;
	//North
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if(Hit.bBlockingHit)
	{
		Neighbours.Add(Cast<ATile>(Hit.Actor));
	}
	//South
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * -60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if(Hit.bBlockingHit)
	{
		Neighbours.Add(Cast<ATile>(Hit.Actor));
	}
	//East
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if(Hit.bBlockingHit)
	{
		Neighbours.Add(Cast<ATile>(Hit.Actor));
	}
	//West
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * -60, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, IgnoreList, EDrawDebugTrace::ForOneFrame, Hit, true);
	if(Hit.bBlockingHit)
	{
		Neighbours.Add(Cast<ATile>(Hit.Actor));
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
	GameMode->HighlightedTile = this;
}
void ATile::ToggleMaterialHidden()
{
	MouseOverMesh->SetVisibility(false);
	bIsHighLighted = false;
}

void ATile::ResetValues()
{
	G_Value = 1;
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
		if(GameMode->HighlightedTile == this)
		{
			GameMode->HighlightedTile = nullptr;
		}
	}
}

