#pragma once

#include "GameFramework/Actor.h"
#include "FGGrid_Highlighters.generated.h"

UCLASS()
class AFGGrid_Highlighters : public AActor
{
	GENERATED_BODY()
public:
	AFGGrid_Highlighters();

	UPROPERTY()
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;

public:

	UFUNCTION(BlueprintCallable)
	void SetMaterial(UMaterialInterface* NewMat);
	
};
