#pragma once

#include "GameFramework/Character.h"
#include "../Enums/AbilityStateEnum.h"
#include "PlayerCharacter.generated.h"


//TO DO: 
//-Clean up this whole class, has a lot of unused functionality

UCLASS( Blueprintable )
class GROUP8_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

		//Inherited functions
public:

	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent( UInputComponent* InputComponent ) override;	

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnWalkingOffLedge_Implementation(const FVector& PreviousFloorImpactNormal, const FVector& PreviousFloorContactNormal, 
		const FVector& PreviousLocation, float TimeDelta) override;

//Get functions
public:

	class UDashComponent* GetDashComponent() const { return DashComponent; }
	class UAbilityStateComponent* GetAbilityStateComponent() { return AbilityStateComponent; }
	class UHandlePlayerCameraComponent* GetHandlePlayerCameraComponent() { return CameraComponent; }
	class UArrowComponent* GetCameraDefaultDirectionArrow() { return CameraDefaultDirection; }
	class UHandlePlayerMovementComponent* GetHandleMovementComponent() { return MoveComponent; }

//Components
public:
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	class UCharacterRotationComponent* RotationComponent = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	class UDashComponent* DashComponent = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	class UAbilityStateComponent* AbilityStateComponent = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	class UHandlePlayerMovementComponent* MoveComponent = nullptr;
	
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	class USpringArmComponent* CameraBoom = nullptr;

	UPROPERTY( VisibleAnywhere )
	class UHandlePlayerCameraComponent* CameraComponent = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	class UArrowComponent* CameraDefaultDirection = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
	class UCameraComponent* Camera = nullptr;

protected:

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	class UStaticMeshComponent* Body = nullptr;

	//Variables
public:

	class APlayerController* PC = nullptr;

	//Functions
public:

	UFUNCTION( BlueprintCallable )
	void DisablePlayerInput();

	UFUNCTION( BlueprintCallable )
	void EnablePlayerInput();

	//Used for debugging state in blueprints
	UFUNCTION( BlueprintImplementableEvent )
	void BP_PrintEnum( EAbilityState EnumToPrint );

	//BP Events abilities
	UFUNCTION( BlueprintImplementableEvent )
	void OnDash();

	UFUNCTION( BlueprintImplementableEvent )
	void OnAirDash();

	UFUNCTION( BlueprintImplementableEvent )
	void OnHover();

	//BP Events other states
	UFUNCTION( BlueprintImplementableEvent )
	void OnJump();

	UFUNCTION( BlueprintImplementableEvent )
	void OnFalling();


	UFUNCTION()
	void AbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState );

};