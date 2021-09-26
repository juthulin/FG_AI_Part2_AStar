#include "FGGridBlockVisualizer.h"
// #include "FGAI_2/Grid/FGGrid_BlockComponent.h"
#include "DrawDebugHelpers.h"

void FFGGridBlockVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	// const UFGGrid_BlockComponent* GridBlockComponent = Cast<const UFGGrid_BlockComponent>(Component);
	
	// if (GridBlockComponent != nullptr && GridBlockComponent->GetOwner() != nullptr)
	// {
	// 	const FMatrix Matrix = GridBlockComponent->GetBlockTransform().ToMatrixNoScale();
	// 	const FVector ExtentsHalf = GridBlockComponent->HalfExtents;
	// 	const FBox Box(-ExtentsHalf, ExtentsHalf);
	// 	::DrawWireBox(PDI, Matrix, Box, FLinearColor::Green, 0);
	// }
}
