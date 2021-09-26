#include "FGGrid_Highlighters.h"

AFGGrid_Highlighters::AFGGrid_Highlighters()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->SetupAttachment(SceneComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void AFGGrid_Highlighters::SetMaterial(UMaterialInterface* NewMat)
{
	MeshComp->SetMaterial(0, NewMat);
}
