#pragma once

#include "ComponentVisualizer.h"

class UActorComponent;

class FGAI_2EDITOR_API FFGGridBlockVisualizer : public FComponentVisualizer
{
	/** Draw visualization for the supplied component */
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};
