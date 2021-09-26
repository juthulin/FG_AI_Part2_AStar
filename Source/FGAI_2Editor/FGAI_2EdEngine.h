#pragma once

#include "Editor/UnrealEdEngine.h"
#include "FGAI_2EdEngine.generated.h"

UCLASS()
class FGAI_2EDITOR_API UFGAI_2EdEngine : public UUnrealEdEngine
{
	GENERATED_BODY()
public:
	virtual void Init(IEngineLoop* InEngineLoop) override;

private:
	TSharedPtr<class FFGGridBlockVisualizer> GridBlockVisualizer;
};
