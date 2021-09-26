#include "FGAI_2EdEngine.h"

// #include "FGAI_2EditorModule.h"
// #include "UnrealEd.h"
#include "FGGridBlockVisualizer.h"
#include "FGAI_2/Grid/FGGrid_BlockComponent.h"

// IMPLEMENT_GAME_MODULE(FFGAI_2EditorModule, FGAI_2Editor)

void UFGAI_2EdEngine::Init(IEngineLoop* InEngineLoop)
{
	Super::Init(InEngineLoop);

	// GridBlockVisualizer = MakeShareable(new FFGGridBlockVisualizer);

	// RegisterComponentVisualizer(UFGGrid_BlockComponent::StaticClass()->GetFName(), GridBlockVisualizer);
}
