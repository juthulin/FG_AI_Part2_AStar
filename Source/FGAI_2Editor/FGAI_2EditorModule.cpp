#include "FGAI_2EditorModule.h"

// #include "UnrealEd.h"
// #include "FGGridBlockVisualizer.h"
// #include "FGAI_2/Grid/FGGrid_BlockComponent.h"

// IMPLEMENT_GAME_MODULE(FFGAI_2EditorModule, FGAI_2Editor)

void FFGAI_2EditorModule::StartupModule()
{
	// if (GUnrealEd)
	// {
	// 	TSharedPtr<FComponentVisualizer> Grid_Block_Visualizer = MakeShareable(new FFGGridBlockVisualizer());
	//
	// 	GUnrealEd->RegisterComponentVisualizer(UFGGrid_BlockComponent::StaticClass()->GetFName(), Grid_Block_Visualizer);
	// 	Grid_Block_Visualizer->OnRegister();
	// }
}

void FFGAI_2EditorModule::ShutdownModule()
{
	// if (GUnrealEd)
	// {
	// 	GUnrealEd->UnregisterComponentVisualizer(UFGGrid_BlockComponent::StaticClass()->GetFName());
	// }
}
