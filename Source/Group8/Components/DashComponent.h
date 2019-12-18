#pragma once


#include "Components/SphereComponent.h"
#include "../Enums/AbilityStateEnum.h"
#include "DashComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnDash );


UCLASS( Blueprintable )
class GROUP8_API UDashComponent : public USphereComponent
{

	GENERATED_BODY()

		//Inherited functions
public:

	UDashComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	//Getter functions
public:

	float GetDashCooldown() { return DashCooldown; }

	bool IsOverlappingDashable();

	//Variables
public:

	UPROPERTY( EditAnywhere, BlueprintReadWrite )
	TSubclassOf<class ADashInteractableActor> Bush;

	UPROPERTY( BlueprintAssignable, Category = Events )
	FOnDash OnDashHit;

	UPROPERTY( BlueprintAssignable, Category = Events )
	FOnDash OnDash;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Settings )
	bool bIsActivated = false;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Settings )
	float DashSpeed = 1000.f;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( UIMin = "0.1", UIMax = "1.0" ), Category = Settings )
	float DashDuration = 0.5f;

	//The time in Seconds it takes from when the Dash starts to when the player can dash again
	UPROPERTY( EditAnywhere, BlueprintReadWrite, meta = ( UIMin = 0.1f, UIMax = 10.f ) )
	float DashCooldown = 2.f;

	bool bIsDashing = false;

	bool bShortDistanceDash = false;

	class APlayerCharacter* OwningChar = nullptr;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly )
	FVector NewVelocity = FVector::ZeroVector;

	class ADashInteractableActor* HitDashable = nullptr;
	//Variables
private:
	FTimerHandle StopDashHandle;
	FTimerHandle DashHandle;

	bool bHasIncreasedDashSpeed = false;

	FVector TargetVelocity = FVector::ZeroVector;

	float LerpAlpha = 0.f;
	float MaxWalkSpeed = 400.f;

	FVector TargetLocation = FVector::ZeroVector;
	FVector InitialLocation = FVector::ZeroVector;
	bool bBool = false;


	//Functions
public:

	void SetAbilityActive();

	UFUNCTION()
	void AbilityStateChanged( EAbilityState NewState, EAbilityState PreviousState );

	void DashHit( class ADashInteractableActor* HitActor, class APlayerCharacter* Char );
	void Dash();
	void StopDash();

	void OnDashStart();

	UFUNCTION()
	void OnDashBeginOverlap( UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult );

	UFUNCTION()
	void OnDashEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );

private:

	void MovementSpeedLerp( float DeltaTime );

	void ShortDistanceDashLerp( float DeltaTime );

	void SetVariables();
	void ResetVariables();

};