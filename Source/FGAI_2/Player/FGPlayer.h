#pragma once

#include "GameFramework/Pawn.h"
#include "FGPlayer.generated.h"

class UCameraComponent;
class AFGGridMother;
class AFGGrid_Cell;

UCLASS()
class AFGPlayer : public APawn
{
	GENERATED_BODY()
public:
	AFGPlayer();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	AFGGridMother* GridMother;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	AFGGrid_Cell* HoveredCell = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Debug")
	AFGGrid_Cell* SelectedCell = nullptr;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    float HorizontalMovementSpeed = 3000.0f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player")
    float VerticalMovementSpeed = 1000.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "Player", meta = (DisplayName = "OnConfirm"))
	void BP_OnConfirm();

	UFUNCTION()
	void CheckMouseHover();

	UFUNCTION()
	bool GetGridMother();

private:
	void UpdateMovement(float DeltaTime);

	void Handle_Forward(float Value);
	void Handle_Right(float Value);
	void Handle_Up(float Value);
	void Handle_ConfirmedPressed();

	bool CheckHoveredCell(AFGGrid_Cell* Cell);
	void SetHoveredCell(AFGGrid_Cell* Cell);
	void RemoveHoveredCell();

	FVector InputVector;
};
