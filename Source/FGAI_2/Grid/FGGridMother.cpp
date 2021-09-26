#include "FGGridMother.h"

#include "FGGrid.h"
#include "FGGrid_Cell.h"

AFGGridMother::AFGGridMother()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;
}

void AFGGridMother::BeginPlay()
{
	Super::BeginPlay();
	
	if (Highlighter_To_Use != nullptr)
	{
		FTransform SpawnTransform = GetActorTransform();
		SpawnTransform.SetLocation(FVector(0.f, 0.f, -1000.f));
	
		Hovered_Highlight = GetWorld()->SpawnActor<AFGGrid_Highlighters>(Highlighter_To_Use, SpawnTransform);
		Hovered_Highlight->SetMaterial(HoveredCell_Mat);
	
		Start_Highlight = GetWorld()->SpawnActor<AFGGrid_Highlighters>(Highlighter_To_Use, SpawnTransform);
		Start_Highlight->SetMaterial(StartCell_Mat);

		Goal_Highlight = GetWorld()->SpawnActor<AFGGrid_Highlighters>(Highlighter_To_Use, SpawnTransform);
		Goal_Highlight->SetMaterial(GoalCell_Mat);
	}
}

void AFGGridMother::SetHoveredCell(AFGGrid_Cell* Cell)
{
	if (Cell == nullptr || Cell == HoveredCell) return;

	HoveredCell = Cell;

	if (Hovered_Highlight)
	{
		Hovered_Highlight->SetActorLocation(HoveredCell->GetActorLocation());
	}
}

void AFGGridMother::RemoveHoveredCell()
{
	HoveredCell = nullptr;
	if (Hovered_Highlight)
	{
		Hovered_Highlight->SetActorLocation(FVector(0.f, 0.f, -1000.f));
	}
}

void AFGGridMother::SelectCell(AFGGrid_Cell* Cell)
{
	if (Cell == nullptr || Cell == StartCell || Cell->CellAccess == ECellAccessibility::Blocked) return;

	if (StartCell == nullptr)
	{
		SetStartCell(Cell);
	}
	else
	{
		SetGoalCell(Cell);
		// run A*? or make a button for that?
		Grids[0]->FindPath(StartCell, GoalCell);
	}
}

void AFGGridMother::SetStartCell(AFGGrid_Cell* Cell)
{
	StartCell = Cell;
	if (Start_Highlight)
	{
		Start_Highlight->SetActorLocation(StartCell->GetActorLocation());
	}
}

void AFGGridMother::SetGoalCell(AFGGrid_Cell* Cell)
{
	GoalCell = Cell;
	if (Goal_Highlight)
	{
		Goal_Highlight->SetActorLocation(GoalCell->GetActorLocation());
	}
}

void AFGGridMother::RemoveStartGoalCells()
{
	RemoveStartCell();
	RemoveGoalCell();
	Grids[0]->ResetGrid();
}

void AFGGridMother::RemoveStartCell()
{
	StartCell = nullptr;
	if (Start_Highlight)
	{
		Start_Highlight->SetActorLocation(FVector(0.f, 0.f, -1000.f));
	}
}

void AFGGridMother::RemoveGoalCell()
{
	GoalCell = nullptr;
	if (Goal_Highlight)
	{
		Goal_Highlight->SetActorLocation(FVector(0.f, 0.f, -1000.f));
	}
}
