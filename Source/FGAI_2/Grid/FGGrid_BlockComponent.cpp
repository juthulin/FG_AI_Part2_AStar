#include "FGGrid_BlockComponent.h"

#include "FGAI_2/Grid/FGGrid.h"

UFGGrid_BlockComponent::UFGGrid_BlockComponent()
{
	bWantsOnUpdateTransform = true;
}

FTransform UFGGrid_BlockComponent::GetBlockTransform() const // Unnecessary?
{
	UE_LOG(LogTemp, Log, TEXT("UFGGrid_BlockComponent::GetBlockTransform() was called! <---<---<"))
	
	if (GetOwner() == nullptr) return FTransform::Identity;

	FVector CustomRelativeLocation = GetRelativeLocation();
	CustomRelativeLocation.Z = 0.f;

	const FVector TransformedRelativeLocation_ZeroZ = GetOwner()->GetActorTransform().TransformPositionNoScale(CustomRelativeLocation);

	return FTransform(GetOwner()->GetActorRotation(), TransformedRelativeLocation_ZeroZ);
}

void UFGGrid_BlockComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	CheckZLocation();
	
	CheckBlockingCells();

	CheckScale();
	// check if blocking cells
}

void UFGGrid_BlockComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	CheckZLocation();

	CheckBlockingCells();

	CheckScale();
	// check if blocking cells
}

void UFGGrid_BlockComponent::CheckZLocation()
{
	FVector RelLoc = GetRelativeLocation();
	if (RelLoc.Z != 0.f)
	{
		RelLoc.Z = 0.f;
		SetRelativeLocation(RelLoc);
	}
}

void UFGGrid_BlockComponent::CheckBlockingCells()
{
	auto* Grid = Cast<AFGGrid>(GetOwner());
	if (!Grid) return;

	Grid->CheckAllBlocks();
}

void UFGGrid_BlockComponent::CheckScale()
{
	if (GetRelativeScale3D() != FVector::OneVector)
	{
		HalfExtents += (GetRelativeScale3D() - FVector::OneVector) * FVector(50.f, 50.f, 0.f);
		SetRelativeScale3D(FVector::OneVector);
	}
}
