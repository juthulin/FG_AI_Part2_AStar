#pragma once

#include "GameFramework/Actor.h"
#include "FGGrid_Cell.generated.h"

UENUM()
enum ECellType
{
	Light		UMETA(DisplayName = "Light"),
	Medium		UMETA(DisplayName = "Medium"),
	Heavy		UMETA(DisplayName = "Heavy")
};

UENUM()
enum ECellAccessibility
{
	Open		UMETA(DisplayName = "Open"),
	Blocked		UMETA(DisplayName = "Blocked")
};

UCLASS()
class AFGGrid_Cell : public AActor
{
	GENERATED_BODY()
public:
	AFGGrid_Cell();
	// virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UPROPERTY()
	USceneComponent* SceneComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ArrowMeshComp;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cell Properties")
	TEnumAsByte<ECellAccessibility> CellAccess;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cell Properties")
	TEnumAsByte<ECellType> CellType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cell Properties")
	FVector2D Index;
	
	UFUNCTION()
	void SetCellAccessibility(ECellAccessibility NewAccess);

	UFUNCTION()
	void ResetCell();

	UFUNCTION()
	void SetPathParent(AFGGrid_Cell* Parent);
	
	UFUNCTION()
	void SetG_Score(const int NewG_Score);
	
	UFUNCTION()
	void SetH_Score(const int NewH_Score);
	
	UFUNCTION()
	void SetF_Score(const int NewF_Score);

	UFUNCTION()
	void CalcAndSetG_Score(const int CurrentG_Score);
	
	UFUNCTION()
	void CalcAndSetF_Score();

	UFUNCTION()
	AFGGrid_Cell* GetPathParent() const { return PathParent; }

	UFUNCTION()
	int GetG_Score() const { return G_Score; }
	
	UFUNCTION()
	int GetH_Score() const { return H_Score; }
	
	UFUNCTION()
	int GetF_Score() const { return F_Score; }

	UFUNCTION()
	void ActivatePathVisual() const;

	UFUNCTION()
	void IsInOpen();
	UFUNCTION()
	void IsInClosed();
	UFUNCTION()
	void IsPath();

	UPROPERTY(VisibleAnywhere, Category="Cell Properties")
	int H_Score = -1;
	
	UPROPERTY(VisibleAnywhere, Category="Cell Properties")
	int G_Score = -1;
	
	UPROPERTY(VisibleAnywhere, Category="Cell Properties")
	int F_Score = -1;
	
	UPROPERTY(VisibleAnywhere, Category="Cell Properties")
	AFGGrid_Cell* PathParent;

private:
	
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	UMaterialInterface* CellType_Blocked_Material;
	
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	UMaterialInterface* CellType_Light_Material;
	
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	UMaterialInterface* OpenList_Material;
	
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	UMaterialInterface* ClosedList_Material;
	
	UPROPERTY(EditDefaultsOnly, Category="Materials")
	UMaterialInterface* Path_Material;

	void SetMaterial() const;
};
