#include "DashInteractableActor.h"
#include "Player/PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"

ADashInteractableActor::ADashInteractableActor()
{
	MeshRoot = CreateDefaultSubobject<UStaticMeshComponent>("MeshRoot");
	RootComponent = MeshRoot;
}

void ADashInteractableActor::DashHit_Implementation(APlayerCharacter* Char)
{
	
}

