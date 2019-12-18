#pragma once

#include "Components/ActorComponent.h"

#include "Enums/AbilityStateEnum.h"
#include "CharacterRotationComponent.generated.h"

//TO DO
//Clean up stuff
UCLASS()
class UCharacterRotationComponent : public UActorComponent
{

	GENERATED_BODY()

public:
	UCharacterRotationComponent();

	//Inherited Functions
public:

	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	//Variables
public:

	class APlayerCharacter* OwningChar = nullptr;

	bool bInputDisabled = false;

	bool bLerpCharacter = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float LerpSpeed = 7.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	float StartWalkingLerpSpeed = 16.f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentLerpSpeed = 3.f;
	//Variables
private:

	bool bAccelerationLerp = false;

	//Functions
private:

	void LerpCharacter( float DeltaTime );

	//Calculates the movement direction depending on player input
	FVector GetDirectionVector();

	void SetLerpState();

	UFUNCTION()
	void StartedWalking();

	bool bRightInput = false;
	bool bForwardInput = false;

};