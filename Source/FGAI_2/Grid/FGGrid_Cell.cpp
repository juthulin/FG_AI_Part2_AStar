#include "FGGrid_Cell.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFGGrid_Cell::AFGGrid_Cell()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(SceneComp);

	ArrowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMeshComponent"));
	ArrowMeshComp->SetupAttachment(SceneComp);

	CellType_Blocked_Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("CellType Blocked Material"));
	CellType_Light_Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("CellType Light Material"));
	OpenList_Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("OpenList Material"));
	ClosedList_Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("ClosedList Material"));
	Path_Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Path Material"));
}

// void AFGGrid_Cell::OnConstruction(const FTransform& Transform)
// {
// 	Super::OnConstruction(Transform);
//
// 	SetMaterial();
// }

void AFGGrid_Cell::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SetMaterial();
}

void AFGGrid_Cell::SetCellAccessibility(ECellAccessibility NewAccess)
{
	if (NewAccess == CellAccess) return;

	CellAccess = NewAccess;
	SetMaterial();
}

void AFGGrid_Cell::ResetCell()
{
	H_Score = -1;
	G_Score = -1;
	F_Score = -1;
	PathParent = nullptr;
	ArrowMeshComp->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
	ArrowMeshComp->SetVisibility(false);
	SetMaterial();
}

void AFGGrid_Cell::SetPathParent(AFGGrid_Cell* Parent)
{
	PathParent = Parent;
}

void AFGGrid_Cell::SetG_Score(const int NewG_Score)
{
	G_Score = NewG_Score * 10;
}

void AFGGrid_Cell::SetH_Score(const int NewH_Score)
{
	H_Score = NewH_Score * 10;
}

void AFGGrid_Cell::SetF_Score(const int NewF_Score)
{
	F_Score = NewF_Score;
}

void AFGGrid_Cell::CalcAndSetG_Score(const int CurrentG_Score)
{
	G_Score = CurrentG_Score + CellType + 5;
}

void AFGGrid_Cell::CalcAndSetF_Score()
{
	F_Score = G_Score + H_Score;
}

void AFGGrid_Cell::ActivatePathVisual() const
{
	if (PathParent)
	{
		const FVector TargetDirection = (PathParent->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		ArrowMeshComp->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(TargetDirection));
		ArrowMeshComp->SetVisibility(true);
	}
}

void AFGGrid_Cell::IsInOpen()
{
	MeshComp->SetMaterial(0, OpenList_Material);
}

void AFGGrid_Cell::IsInClosed()
{
	MeshComp->SetMaterial(0, ClosedList_Material);
}

void AFGGrid_Cell::IsPath()
{
	MeshComp->SetMaterial(0, Path_Material);
}

void AFGGrid_Cell::SetMaterial() const
{
	if (CellAccess == ECellAccessibility::Blocked)
	{
		MeshComp->SetMaterial(0, CellType_Blocked_Material);
	}
	else
	{
		MeshComp->SetMaterial(0, CellType_Light_Material);
	}
}
