#pragma once

#include "GameFramework/Actor.h"
#include "DashInteractableActor.h"
#include "DashPushInteractableActor.generated.h"

UCLASS(BlueprintType)
class GROUP8_API ADashPushInteractableActor : public ADashInteractableActor
{
	GENERATED_BODY()

//Inheritable functions
public:

	ADashPushInteractableActor();

	virtual void DashHit_Implementation(class APlayerCharacter* Char) override;

//Variables
public:

	UPROPERTY(EditAnywhere)
	float Weight = 1.f;

};