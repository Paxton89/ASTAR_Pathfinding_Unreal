//Author: Johan Liljedahl

#include "FG_ASTAR/Camera/CameraPawn.h"
#include "DrawDebugHelpers.h"
#include "FG_ASTAR/Grid/Tile.h"
#include "Camera/CameraComponent.h"
#include "FG_ASTAR/FG_ASTARGameModeBase.h"
#include "FG_ASTAR/Units/Components/Unit_Movement_Component.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"



ACameraPawn::ACameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Main Camera"));
	MainCamera->SetupAttachment(RootComponent);
}

void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AFG_ASTARGameModeBase>(GetWorld()->GetAuthGameMode());
	IgnoreList.Add(this);
}

void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(FVector(MoveX, MoveY, 0));
	FHitResult Hit;
	FHitResult UnderMouse;
	GameMode->PlayerController->GetHitResultUnderCursor(ECC_WorldDynamic, false, UnderMouse);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), MainCamera->GetComponentLocation(), UnderMouse.Location, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic),false, IgnoreList, EDrawDebugTrace::None, Hit, true);
	if(Hit.bBlockingHit)
	{
		if(Hit.Actor->IsA(ATile::StaticClass()))
		{
			TargetedTile = Cast<ATile>(Hit.Actor);
			TargetedTile->ToggleMaterialVisible();
		}
	}
}

void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &ACameraPawn::LeftClick);
}

void ACameraPawn::EmptyPairedList()
{
	PairedTiles.Empty();
}

void ACameraPawn::MoveForward(float Value)
{
	MoveX = Value;
}
void ACameraPawn::MoveRight(float Value)
{
	MoveY = Value;
}

void ACameraPawn::LeftClick()
{
	
	if(TargetedTile->isIllegal)
	{
		UE_LOG(LogTemp, Error, TEXT("TARGETED TILE IS ILLEGAL!, Aborting"))
		EmptyPairedList();
	}
	else if(TargetedTile != nullptr)
	{
		DrawDebugCylinder(GetWorld(), TargetedTile->GetActorLocation(), TargetedTile->GetActorLocation() + TargetedTile->GetActorUpVector() * 30, 10, 18, FColor::Yellow, false, 6, 0, 1);
		PairedTiles.Add(TargetedTile);
		if(PairedTiles.Num() == 1) //If only 1 element
		{
			StartTile = PairedTiles.Top();
		}
		else if(PairedTiles.Num() == 2 && PairedTiles.Top() != StartTile) // If List has 2 elements
		{
			EndTile = PairedTiles.Top();
			GameMode->CalculatePath(StartTile, EndTile);
			EmptyPairedList();
		}
		else if(PairedTiles.Num() > 2)
		{
			EmptyPairedList();
			UE_LOG(LogTemp, Warning, TEXT("PairedTilesList has more than 2 elements, Aborting"))
		}
		else if(PairedTiles.Top() == StartTile)
		{
			UE_LOG(LogTemp, Warning, TEXT("Start Tile & End tile may not be the same, Aborting"))
			EmptyPairedList();
		}
	}
}
