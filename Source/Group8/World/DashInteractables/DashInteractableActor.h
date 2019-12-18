#pragma once

#include "GameFramework/Actor.h"
#include "DashInteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnDashInteractableStateChanged );


UCLASS(BlueprintType)
class GROUP8_API ADashInteractableActor : public AActor
{

	GENERATED_BODY()

//Inheritable functions
public:

	ADashInteractableActor();

	UFUNCTION( BlueprintNativeEvent, Category = Dash )
	void DashHit(class APlayerCharacter* Char);

	UFUNCTION()
	virtual void DashHit_Implementation(class APlayerCharacter* Char);

//Components
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshRoot = nullptr;

	//Functions
public:

	//Called when the player destroys this object
	UPROPERTY(BlueprintCallable)
	FOnDashInteractableStateChanged OnDestroyedByPlayer;


};