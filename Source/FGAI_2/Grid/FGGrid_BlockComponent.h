#pragma once

#include "Components/SceneComponent.h"
#include "FGGrid_BlockComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class UFGGrid_BlockComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	UFGGrid_BlockComponent();

	UFUNCTION(BlueprintPure)
	FTransform GetBlockTransform() const; // Unnecessary?

	UPROPERTY(EditAnywhere)
	FVector HalfExtents = FVector(500.f, 500.f, 100.f);

	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) override;

private:
	
	void CheckZLocation();
	void CheckBlockingCells();
	void CheckScale();
};
