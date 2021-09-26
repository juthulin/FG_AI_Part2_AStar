#pragma once

#include "FGGrid_Highlighters.h"
#include "GameFramework/Actor.h"
#include "FGGridMother.generated.h"

class AFGGrid;
class AFGGrid_Cell;
class AFGGrid_Highlighters;

UCLASS()
class AFGGridMother : public AActor
{
	GENERATED_BODY()
public: // Unreal public
	AFGGridMother();
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly, Category="Highlighter")
	UMaterialInterface* HoveredCell_Mat;
	
	UPROPERTY(EditDefaultsOnly, Category="Highlighter")
	UMaterialInterface* StartCell_Mat;
	
	UPROPERTY(EditDefaultsOnly, Category="Highlighter")
	UMaterialInterface* GoalCell_Mat;

public: // My public

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	TArray<AFGGrid*> Grids;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	AFGGrid_Cell* HoveredCell;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	AFGGrid_Cell* StartCell;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	AFGGrid_Cell* GoalCell;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	AFGGrid_Highlighters* Hovered_Highlight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	AFGGrid_Highlighters* Start_Highlight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug") // Visible for debug purposes.
	AFGGrid_Highlighters* Goal_Highlight;

	UPROPERTY(EditDefaultsOnly, Category="Highlighter")
	TSubclassOf<AFGGrid_Highlighters> Highlighter_To_Use;

	UFUNCTION(BlueprintCallable)
	void SetHoveredCell(AFGGrid_Cell* Cell);

	UFUNCTION(BlueprintCallable)
	void RemoveHoveredCell();
	
	UFUNCTION(BlueprintCallable)
	void SelectCell(AFGGrid_Cell* Cell);

	UFUNCTION(BlueprintCallable)
	void RemoveStartGoalCells();

private: // My private

	void SetStartCell(AFGGrid_Cell* Cell);
	void SetGoalCell(AFGGrid_Cell* Cell);
	void RemoveStartCell();
	void RemoveGoalCell();
	
	// list of cell path visuals?
	// 
};
