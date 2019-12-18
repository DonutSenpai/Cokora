#pragma once

#include "../../Enums/AbilityStateEnum.h"
#include "Components/ActorComponent.h"
#include "AbilityStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FAbilityStateChanged, EAbilityState, NewState, EAbilityState, PreviousState );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnHover, class AUpdraftBase*, Updraft );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FAbilityStateChangedAnimation );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FDashAbilityStateChangedAnimation, EAbilityState, DashState );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FAbilityStateChangedAnimationIsMoving, bool, IsMoving );

UCLASS()
class GROUP8_API UAbilityStateComponent : public UActorComponent
{

	GENERATED_BODY()


		//Inherited functions & constructor
public:

	UAbilityStateComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	/*UFUNCTION()
	void OnCharacterBeginOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult );

	UFUNCTION()
	void OnEndOverlap( class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex );*/

	//Getters
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EAbilityState GetAbilityState() const { return CurrentAbilityState; }

	FORCEINLINE EAbilityState GetPreviousAbilityState() const { return PreviousAbilityState; }

	UFUNCTION(BlueprintCallable)
	void ActivateJump();
	//Variables
public:

	class APlayerCharacter* OwningChar = nullptr;

	class AUpdraftBase* UpdraftReference = nullptr;

	bool bInputDisabled = false;

	float DashCooldown = 1.5f;

	UPROPERTY( BlueprintAssignable, Category = Events )
	FOnHover OnHoverStart;

	UPROPERTY( BlueprintAssignable, Category = Events )
	FOnHover OnHoverEnd;

	UPROPERTY( BlueprintAssignable, Category = Events )
	FAbilityStateChanged AbilityStateChanged;

	//True = Player can jump, False = Player can't jump
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bJumpIsActivated = true;

	//Variables
protected:
	EAbilityState CurrentAbilityState = EAbilityState::AS_None;
	EAbilityState PreviousAbilityState = EAbilityState::AS_None;

	FTimerDelegate DashEndTimerDelegate;
	FTimerHandle DashEndHandle;
	FTimerDelegate HoverTimerDelegate;
	FTimerHandle HoverHandle;
	bool bHasDashed = false;
	bool bHasHovered = false;

	float DashCooldownTimer = 0.f;

	bool bIsDashing = false;

	//Functions
public:

	void JumpInput();
	void DashInput();
	void OnLanded();
	void WalkedOffLedge();

	//returns true if successfully changed the state to hover
	UFUNCTION()
	bool HoverStart( class AUpdraftBase* Updraft );

	UFUNCTION()
	void HoverEnd( class AUpdraftBase* Updraft );

	//BE CAUTIOUS when using this event
	void SetStateToNone();

	//BE CAUTIOUS when using this event
	void SetStateToWalking();

	//Blueprint Event functions to be used for proper animation stuff
public:

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FDashAbilityStateChangedAnimation AnimationEventOnDash;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FAbilityStateChangedAnimation AnimationEventOnDashHit;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FDashAbilityStateChangedAnimation AnimationEventOnDashEnd;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FAbilityStateChangedAnimation AnimationEventOnStartedWalking;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FAbilityStateChangedAnimationIsMoving AnimationEventOnStoppedWalking;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FAbilityStateChangedAnimation AnimationEventOnJump;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FAbilityStateChangedAnimation AnimationEventOnStartHover;

	UPROPERTY( BlueprintAssignable, Category = "Animation Event" )
	FAbilityStateChangedAnimation AnimationEventOnEndHover;

	//Called when the player completely stops moving and doesn't do anything
	UPROPERTY( BlueprintAssignable, Category = "Animation Event")
	FAbilityStateChangedAnimation AnimationEventOnIdle;


	//Functions
private:

	UFUNCTION()
	void OnStartedWalkingInternal();
	
	UFUNCTION()
	void OnStoppedWalkingInternal();

	UFUNCTION()
	void DashEnd();

	void ChangeAbilityState( EAbilityState NewState );

	UFUNCTION()
	void OnDashHitInternal();






};