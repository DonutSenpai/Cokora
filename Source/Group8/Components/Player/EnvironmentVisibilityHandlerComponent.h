#pragma once

#include "Components/SphereComponent.h"
#include "EnvironmentVisibilityHandlerComponent.generated.h"

UCLASS()
class GROUP8_API UEnvironmentVisibilityHandlerComponent : public USphereComponent
{

	GENERATED_BODY()//

	//Inherited functions & constructor
public:

	UEnvironmentVisibilityHandlerComponent();

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction );

	//Variables
private:

	TArray<AActor> OverlappingActors = {};
	TArray<UPrimitiveComponent> OverlappingComponents = {};


	//Functions
public:

	void SetOverlappingActors();
	void SetOverlappingComponents();


};