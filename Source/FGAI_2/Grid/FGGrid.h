#pragma once

#include "FGGrid_BlockComponent.h"
#include "GameFramework/Actor.h"
#include "FGGrid.generated.h"

class AFGGrid_Cell;

UCLASS()
class AFGGrid : public AActor
{
	GENERATED_BODY()
public:
	AFGGrid();
	// virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;

public:

	UPROPERTY(VisibleAnywhere)
	TArray<AFGGrid_Cell*> Cells;
	
	UPROPERTY(EditAnywhere)
	int DimensionX = 10;

	UPROPERTY(EditAnywhere)
	int DimensionY = 10;
	
	UPROPERTY(EditAnywhere)
	float CellSpacing = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AFGGrid_Cell> CellClass;

	UPROPERTY(EditAnywhere)
	float PathFindingSpeed = .2f;

	UFUNCTION(BlueprintCallable)
	void InitializeGrid();

	UFUNCTION()
	void CheckAllBlocks();

	UFUNCTION()
	void ResetGrid();

	UFUNCTION()
	void FindPath(AFGGrid_Cell* Start, /*const */AFGGrid_Cell* Goal);

	void ProcessCell(/*AFGGrid_Cell* CurrentCell, const AFGGrid_Cell* Goal*/);

	TArray<AFGGrid_Cell*> FindCellNeighbors(AFGGrid_Cell* Cell);

	int CalculateManhattanDistance(const AFGGrid_Cell* FromCell, const AFGGrid_Cell* ToCell) const;

private:

	TArray<AFGGrid_Cell*> Open;
	TArray<AFGGrid_Cell*> Closed;

	UPROPERTY()
	AFGGrid_Cell* M_Start;
	UPROPERTY()
	AFGGrid_Cell* M_Goal;
	UPROPERTY()
	AFGGrid_Cell* CurrentCell;

	void InsertCellInArray(TArray<AFGGrid_Cell*>& Array, AFGGrid_Cell* Cell);
	void SetParentAndScores(AFGGrid_Cell* Cell, AFGGrid_Cell* Parent, const AFGGrid_Cell* Goal);
	
	bool bIsInitialized = false;
	
	void GenerateGrid();
	void ExtendGrid();
	void ReduceGrid();
	void MoveGrid();
	FVector GetGridCenterLocation() const;
};
