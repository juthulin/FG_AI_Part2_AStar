#include "FGPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "FGAI_2/Grid/FGGrid.h"
#include "FGAI_2/Grid/FGGrid_Cell.h"
#include "FGAI_2/Grid/FGGridMother.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AFGPlayer::AFGPlayer()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(RootComponent);
}

void AFGPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetGridMother();
}

void AFGPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateMovement(DeltaSeconds);

	CheckMouseHover();
}

void AFGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Forward"), this, &AFGPlayer::Handle_Forward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AFGPlayer::Handle_Right);
	PlayerInputComponent->BindAxis(TEXT("Up"), this, &AFGPlayer::Handle_Up);

	PlayerInputComponent->BindAction(TEXT("Confirm"), IE_Pressed, this, &AFGPlayer::Handle_ConfirmedPressed);
}

void AFGPlayer::CheckMouseHover()
{
	const APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	
	FVector MouseWorldLocation;
	FVector MouseDirection;
	PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseDirection);

	const FVector EndLoc = MouseWorldLocation + MouseDirection * 10000.f;
	const FName TraceProfile = TEXT("FGGridCell");
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	FHitResult Hit;
	
	UKismetSystemLibrary::LineTraceSingleByProfile(this, GetActorLocation(), EndLoc, TraceProfile, false,
													IgnoreActors, EDrawDebugTrace::None, Hit, true);

	if (Hit.bBlockingHit)
	{
		auto* Cell = Cast<AFGGrid_Cell>(Hit.Actor);
		if (Cell)
		{
			if (CheckHoveredCell(Cell))
			{
				SetHoveredCell(Cell);
			}

			// UKismetSystemLibrary::DrawDebugSphere(this, Cell->GetActorLocation(), 50.f,
			// 								12, FLinearColor::Red, 0.f, 4.f);
		}
		else
		{
			RemoveHoveredCell();
		}
	}
	else
	{
		RemoveHoveredCell();
	}
}

bool AFGPlayer::GetGridMother()
{
	TArray<AActor*> AllMothers;
	UGameplayStatics::GetAllActorsOfClass(this, AFGGridMother::StaticClass(), AllMothers);

	if (AllMothers.Num() > 0)
	{
		GridMother = Cast<AFGGridMother>(AllMothers[0]);
	}

	return GridMother != nullptr;
}

void AFGPlayer::UpdateMovement(float DeltaTime)
{
	if (!InputVector.IsNearlyZero())
	{
		FVector HorizontalMovement = GetActorTransform().TransformVectorNoScale(FVector(InputVector.X, InputVector.Y, 0.0f).GetSafeNormal());
		FVector VerticalMovement = FVector(0, 0, InputVector.Z);

		AddActorWorldOffset(HorizontalMovement * HorizontalMovementSpeed * DeltaTime);
		AddActorWorldOffset(VerticalMovement * VerticalMovementSpeed * DeltaTime);
	}

	InputVector = FVector::ZeroVector;
}

void AFGPlayer::Handle_Forward(float Value)
{
	InputVector.X += Value;
}

void AFGPlayer::Handle_Right(float Value)
{
	InputVector.Y += Value;
}

void AFGPlayer::Handle_Up(float Value)
{
	InputVector.Z += Value;
}

void AFGPlayer::Handle_ConfirmedPressed()
{
	SelectedCell = HoveredCell;
	
	if (GridMother == nullptr) return;
	
	if (GridMother->HoveredCell != nullptr)
	{
		GridMother->SelectCell(SelectedCell);
	}
	else
	{
		GridMother->RemoveStartGoalCells();
	}
	// BP_OnConfirm();
}

bool AFGPlayer::CheckHoveredCell(AFGGrid_Cell* Cell)
{
	if (GridMother == nullptr) return false;
	
	return Cell != GridMother->HoveredCell;
}

void AFGPlayer::SetHoveredCell(AFGGrid_Cell* Cell)
{
	if (GridMother == nullptr) return;

	HoveredCell = Cell;
	GridMother->SetHoveredCell(Cell);
}

void AFGPlayer::RemoveHoveredCell()
{
	if (GridMother == nullptr) return;

	HoveredCell = nullptr;
	GridMother->RemoveHoveredCell();
}
