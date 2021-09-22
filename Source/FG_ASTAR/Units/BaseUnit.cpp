#include "FG_ASTAR/Units/BaseUnit.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FG_ASTAR/Units/Components/Unit_Movement_Component.h"
ABaseUnit::ABaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = Box;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("NoCollision");

	MovementComponent = CreateDefaultSubobject<UUnit_Movement_Component>("Movement Component");
}

void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

