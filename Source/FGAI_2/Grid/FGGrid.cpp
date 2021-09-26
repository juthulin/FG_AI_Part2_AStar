#include "FGGrid.h"

#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "Chaos/AABB.h"
#include "FGAI_2/Grid/FGGrid_Cell.h"
#include "FGAI_2/Grid/FGGrid_BlockComponent.h"

AFGGrid::AFGGrid()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

// void AFGGrid::OnConstruction(const FTransform& Transform)
// {
// 	Super::OnConstruction(Transform);
//
// 	if (!bIsInitialized) return;
// 	
// 	if (Cells.Num() != DimensionX * DimensionY)
// 		GenerateGrid();
// 	else
// 		MoveGrid();
// }

void AFGGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (!bIsInitialized) return;
	
	if (Cells.Num() != DimensionX * DimensionY)
		GenerateGrid();
	else
		MoveGrid();
}

void AFGGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AFGGrid::InitializeGrid()
{
	bIsInitialized = true;

	GenerateGrid();
}

void AFGGrid::CheckAllBlocks()
{
	// get all blocks
	TArray<AFGGrid_Cell*> BlockedCells;
	TArray<UFGGrid_BlockComponent*> AllBlocks;
	GetComponents(AllBlocks);
	
	// compare locations to determine if cell inside block
	for (auto* Cell : Cells)
	{
		Cell->SetCellAccessibility(ECellAccessibility::Open);
		
		for (const auto* Block : AllBlocks)
		{
			const int UpperX = Block->GetComponentLocation().X + Block->HalfExtents.X;
			const int LowerX = Block->GetComponentLocation().X - Block->HalfExtents.X;
			const int UpperY = Block->GetComponentLocation().Y + Block->HalfExtents.Y;
			const int LowerY = Block->GetComponentLocation().Y - Block->HalfExtents.Y;
			
			const FVector Loc = Cell->GetActorLocation();
			if (Loc.X <= UpperX && Loc.X >= LowerX && Loc.Y <= UpperY && Loc.Y >= LowerY)
			{
				BlockedCells.Add(Cell);
			}
		}
	}

	for (auto* Cell : BlockedCells)
	{
		Cell->SetCellAccessibility(ECellAccessibility::Blocked);
	}
}

void AFGGrid::ResetGrid()
{
	for (auto* Cell : Cells)
	{
		Cell->ResetCell();
	}
	Open.Empty();
	Closed.Empty();
}

void AFGGrid::ProcessCell(/*AFGGrid_Cell* CurrentCell, const AFGGrid_Cell* Goal*/)
{
	if (CurrentCell == M_Goal)
	{
		// we found the goal
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Green,
			TEXT("Finished with Success!"));

		AFGGrid_Cell* Target = M_Goal;
			while (Target != nullptr)
			{
				Target->IsPath();
				Target = Target->GetPathParent();
			}
		
		return;// true;
	}

	CurrentCell->IsPath();

	TArray<AFGGrid_Cell*> Neighbors = FindCellNeighbors(CurrentCell);

	for (auto* Neighbor : Neighbors)
	{
		if (Open.Contains(Neighbor) && Neighbor->GetPathParent() != nullptr)
		{
			if (CurrentCell->GetG_Score() < Neighbor->GetPathParent()->GetG_Score())
			{
				SetParentAndScores(Neighbor, CurrentCell, M_Goal);
					
				Open.Remove(Neighbor);
				InsertCellInArray(Open, Neighbor);
				Neighbor->IsInOpen();
			}
		}
		else if (Closed.Contains(Neighbor))
		{
			continue;
			// possibly check if neighbors f score is better or worse.
		}
		else
		{
			SetParentAndScores(Neighbor, CurrentCell, M_Goal);
			InsertCellInArray(Open, Neighbor);
			Neighbor->IsInOpen();
		}
	}

	Open.Remove(CurrentCell);
	InsertCellInArray(Closed, CurrentCell);
	CurrentCell->IsInClosed();

	if (Open.Num() == 0) // return false;
	{
		GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red,
			TEXT("Finished with failure. Could not find path to Goal!"));
	} 
	else
	{
		CurrentCell = Open[0];
	
		FTimerHandle THandle;
		GetWorldTimerManager().SetTimer(THandle, this, &AFGGrid::ProcessCell, PathFindingSpeed, false);
	}

	
	//return ProcessCell(Open[0], Goal);

	
}

void AFGGrid::FindPath(AFGGrid_Cell* Start, /*const*/ AFGGrid_Cell* Goal)
{
	M_Start = Start;
	M_Goal = Goal;
	
	Start->SetG_Score(0);
	Start->SetH_Score(CalculateManhattanDistance(Start, Goal));
	Start->CalcAndSetF_Score();
	Open.Add(Start);
	CurrentCell = Start;

	ProcessCell(/*Start, Goal*/);
	
	// if (ProcessCell(Start, Goal))
	// {
	// 	// we have a path
	// 	const AFGGrid_Cell* Target = Goal;
	// 	while (Target != nullptr)
	// 	{
	// 		Target->ActivatePathVisual();
	// 		Target = Target->GetPathParent();
	// 	}
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red,
	// 		TEXT("Could not find path to Goal!"));
	// 	// we didn't find goal.
	// }

	// Algorithm finished. Now we can do stuff.

}

TArray<AFGGrid_Cell*> AFGGrid::FindCellNeighbors(AFGGrid_Cell* Cell)
{
	const FVector2D Above {Cell->Index.X, Cell->Index.Y - 1};
	const FVector2D Below {Cell->Index.X, Cell->Index.Y + 1};
	const FVector2D Left {Cell->Index.X - 1, Cell->Index.Y};
	const FVector2D Right {Cell->Index.X + 1, Cell->Index.Y};
	
	TArray<AFGGrid_Cell*> Neighbors;

	int Index;
	if (Above.Y >= 0)
	{
		Index = Above.Y + (DimensionY * Above.X);
		
		if (Cells[Index]->CellAccess != ECellAccessibility::Blocked)
			Neighbors.Add(Cells[Index]);
	}
	if (Below.Y < DimensionY)
	{
		Index = Below.Y + (DimensionY * Below.X);
		
		if (Cells[Index]->CellAccess != ECellAccessibility::Blocked)
			Neighbors.Add(Cells[Index]);
	}
	if (Left.X >= 0)
	{
		Index = Left.Y + (DimensionY * Left.X);
		
		if (Cells[Index]->CellAccess != ECellAccessibility::Blocked)
			Neighbors.Add(Cells[Index]);
	}
	if (Right.X < DimensionX)
	{
		Index = Right.Y + (DimensionY * Right.X);
		
		if (Cells[Index]->CellAccess != ECellAccessibility::Blocked)
			Neighbors.Add(Cells[Index]);
	}

	return Neighbors;
}

int AFGGrid::CalculateManhattanDistance(const AFGGrid_Cell* FromCell, const AFGGrid_Cell* ToCell) const
{
	return abs(FromCell->Index.X - ToCell->Index.X) + abs(FromCell->Index.Y - ToCell->Index.Y);
}

void AFGGrid::InsertCellInArray(TArray<AFGGrid_Cell*>& Array, AFGGrid_Cell* Cell)
{
	if (Array.Num() == 0)
	{
		Array.Add(Cell);
	}
	else
	{
		const int N = Array.Num();
		for (int i = 0; i < N; ++i)
		{
			if (Cell->GetF_Score() < Array[i]->GetF_Score())
			{
				Array.Insert(Cell, i);
				break;
			}

			if (i == Array.Num() - 1)
				Array.Add(Cell);
		}
	}
}

void AFGGrid::SetParentAndScores(AFGGrid_Cell* Cell, AFGGrid_Cell* Parent, const AFGGrid_Cell* Goal)
{
	Cell->SetPathParent(Parent);
	Cell->CalcAndSetG_Score(Parent->GetG_Score());
	Cell->SetH_Score(CalculateManhattanDistance(Cell, Goal));
	Cell->CalcAndSetF_Score();
}

void AFGGrid::GenerateGrid()
{
	if (Cells.Num() < DimensionX * DimensionY)
		ExtendGrid();
	else
		ReduceGrid();
		
	for (int x = 0; x < DimensionX; ++x)
	{
		for (int y = 0; y < DimensionY; ++y)
		{
			const int i = y + (DimensionY * x);
			Cells[i]->Index.X = x;
			Cells[i]->Index.Y = y;
		}
	}

	const FVector NewScale = FVector((DimensionX + 1) + (CellSpacing * DimensionX) * .001f,
									 (DimensionY + 1) + (CellSpacing * DimensionY) * .001f,
									 1.f);
	MeshComp->SetWorldScale3D(NewScale);

	MoveGrid();
}

void AFGGrid::ExtendGrid()
{
	const int Lenght = DimensionX * DimensionY;
	for (int i = Cells.Num(); i < Lenght; ++i)
	{
		auto* Cell = GetWorld()->SpawnActor<AFGGrid_Cell>(CellClass, GetActorTransform());
		Cells.Add(Cell);
	}
}

void AFGGrid::ReduceGrid()
{
	// UE_LOG(LogTemp, Log, TEXT("AFGGrid::ReduceGrid: Cells.Num = %i"), Cells.Num());
	const int Lenght = DimensionX * DimensionY;
	for (int i = Lenght; i < Cells.Num(); ++i)
	{
		// UE_LOG(LogTemp, Log, TEXT("i = %i"), i);
		auto* Cell = Cells[i];
		Cells.Remove(Cell);
		Cell->Destroy();
	}
}

void AFGGrid::MoveGrid()
{
	if (Cells.Num() == 0) return;

	const FVector GridLoc = GetGridCenterLocation();
	for (auto* Cell : Cells)
	{
		FVector NewCellLoc = FVector((Cell->Index.X * CellSpacing) + GridLoc.X,
									 (Cell->Index.Y * CellSpacing) + GridLoc.Y,
									 GridLoc.Z);
		
		Cell->SetActorLocation(NewCellLoc);
	}
}

FVector AFGGrid::GetGridCenterLocation() const
{
	const FVector Offset = FVector((-CellSpacing * DimensionX - 1) * .5f + CellSpacing * .5f,
								(-CellSpacing * DimensionY - 1) * .5f + CellSpacing * .5f,
								50.f);
	
	return GetActorLocation() + Offset;
}

	// while (Open.Num() > 0 && Iterations <= MaxIterations)
	// {
	// 	//TODO: Redo FindPath from a while loop to recursive methods to allow for slowing down the algorithm.
	// 	//TODO: Expose OPEN for debug.
	// 	//TODO: Make some more visuals on the cells for debug.
	// 	
	// 	AFGGrid_Cell* CurrentCell = Open[0];
	//
	// 	// maybe not necessary, since we're sorting as we insert to Open. 
	// 	for (int i = 0; i < Open.Num(); ++i)
	// 	{
	// 		if (Open[i]->GetF_Score() < CurrentCell->GetF_Score())
	// 		{
	// 			AFGGrid_Cell* tmp = CurrentCell;
	// 		    CurrentCell = Open[i];
	// 			Open[i] = tmp;
	// 		}
	// 	}
	//
	// 	// CurrentCell->ActivatePathVisual();
	//
	// 	if (CurrentCell == Goal)
	// 	{
	// 		// Stop the search, we're at our goal.
	// 		// Goal->SetPathParent(CurrentCell);
	// 		break;
	// 	}
	//
	// 	TArray<AFGGrid_Cell*> Neighbors = FindCellNeighbors(CurrentCell);
	//
	// 	for (auto* Neighbor : Neighbors)
	// 	{
	// 		if (Open.Contains(Neighbor))
	// 		{
	// 			if (Neighbor->GetPathParent() != nullptr)
	// 			{
	// 				if (CurrentCell->GetG_Score() < Neighbor->GetPathParent()->GetG_Score())
	// 				{
	// 					Neighbor->SetPathParent(CurrentCell);
	// 					Neighbor->CalcAndSetG_Score(CurrentCell->GetG_Score());
	// 					Neighbor->CalcAndSetF_Score();
	// 					Open.Remove(Neighbor);
	//
	// 					const int N = Open.Num();
	// 					for (int i = 0; i < N; ++i)
	// 					{
	// 						if (Neighbor->GetF_Score() < Open[i]->GetF_Score())
	// 						{
	// 							Open.Insert(Neighbor, i);
	// 							break;
	// 						}
	//
	// 						if (i == Open.Num() - 1)
	// 							Open.Add(Neighbor);
	// 					}
	// 					// sort OPEN again, or insert neighbor at correct place.
	// 				}
	// 			}
	// 		}
	// 		else if (Closed.Contains(Neighbor))
	// 		{
	// 			continue;
	// 			// possibly check if neighbors f score is better or worse.
	// 		}
	// 		else
	// 		{
	// 			Neighbor->SetPathParent(CurrentCell);
	// 			Neighbor->CalcAndSetG_Score(CurrentCell->GetG_Score());
	// 			Neighbor->SetH_Score(CalculateManhattanDistance(Neighbor, Goal));
	// 			Neighbor->CalcAndSetF_Score();
	//
	// 			if (Open.Num() == 0)
	// 			{
	// 				Open.Add(Neighbor);
	// 			}
	// 			else
	// 			{
	// 				const int N = Open.Num();
	// 				for (int i = 0; i < N; ++i)
	// 				{
	// 					if (Neighbor->GetF_Score() < Open[i]->GetF_Score())
	// 					{
	// 						Open.Insert(Neighbor, i);
	// 						break;
	// 					}
	//
	// 					if (i == Open.Num() - 1)
	// 						Open.Add(Neighbor);
	// 				}
	// 			}
	// 		}
	// 	}
	//
	// 	Open.Remove(CurrentCell);
	// 	
	// 	if (Closed.Num() == 0)
	// 	{
	// 		Closed.Add(CurrentCell);
	// 	}
	// 	else
	// 	{
	// 		const int N = Closed.Num();
	// 		for (int i = 0; i < N; ++i)
	// 		{
	// 			if (CurrentCell->GetF_Score() < Closed[i]->GetF_Score())
	// 			{
	// 				Closed.Insert(CurrentCell, i);
	// 				break;
	// 			}
	// 			
	// 			if (i == Closed.Num() - 1)
	// 				Closed.Add(CurrentCell);
	// 		}
	// 	}
	// 	++Iterations;
	// 	if (Iterations >= MaxIterations)
	// 		UE_LOG(LogTemp, Log, TEXT("AFGGrid::FindPath: Something went wrong, ran out of Iterations!"));
	//
	// 	CurrentCell->ActivatePathVisual();
	// }