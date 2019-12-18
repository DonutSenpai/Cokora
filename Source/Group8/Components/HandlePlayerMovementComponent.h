#pragma once

#include "Components/ActorComponent.h"
#include "../Enums/AbilityStateEnum.h"
#include "HandlePlayerMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnMovementChanged );


UCLASS( Blueprintable )
class GROUP8_API UHandlePlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHandlePlayerMovementComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;


	//Getter Functions
public:

	FORCEINLINE bool GetHasForwardInput() const { return bForwardInput; }
	FORCEINLINE bool GetHasRightInput() const { return bRightInput; }

	FORCEINLINE bool GetHasAnyInput() const { return ( bRightInput || bForwardInput ); }

	//Variables
public:

	class APlayerCharacter* OwningChar = nullptr;

	UPROPERTY(BlueprintAssignable, Category = Events)
	FOnMovementChanged OnStartedWalking;

	UPROPERTY( BlueprintAssignable, Category = Events )
	FOnMovementChanged OnStoppedWalking;
	
	
	bool bInputDisabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( UIMin = "0.01", UIMax = "1.0"), Category = Settings)
	float NormalAirControl = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( UIMin = "0.01", UIMax = "1.0"), Category = Settings)
	float JumpAirControl = 0.15f;

	bool bJustStartedMoving = false;

	//Variables
private:
	bool bHasJumped = false;

	//Functions
public:

	UFUNCTION()
	void MoveForward(float InputValue);

	UFUNCTION()
	void MoveRight(float InputValue);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void OnAbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState );

	//Functions
private:
	void EnterJumpState();
	void ExitJumpState();

	void SetDirectionVector();

	FVector MovmentDirection = FVector::ZeroVector;

	float InputValueForward = 0.f;
	float InputValueRight = 0.f;
	bool bForwardInput = false;
	bool bRightInput = false;

	float PreviousInputValForward = 0.f;
	float PreviousInputValRight = 0.f;

};