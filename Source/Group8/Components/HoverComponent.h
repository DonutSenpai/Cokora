#pragma once
#include "Components/ActorComponent.h"
#include "../Enums/AbilityStateEnum.h"
#include "HoverComponent.generated.h"



UCLASS( Blueprintable )
class GROUP8_API UHoverComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UHoverComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	void Hover();
	void StopHover(); 

	void SetAbilityActive();
	
//Variables
public:
	UPROPERTY( EditDefaultsOnly, Category ="Hover Setting" )
	float HoverDuration = 2.f;

	class APlayerCharacter* OwningChar = nullptr;

	class AUpdraftBase* Updraft = nullptr;

	
	
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Ability Is Active" )
	bool bAbilityActivated = false;

//Functions
public:

	UFUNCTION()
	void AbilityStateChanged( EAbilityState NewState );



private:
	FTimerHandle HoverDurationHandle;
	FTimerHandle HoverDelayHandle;


	UPROPERTY( EditDefaultsOnly, Category ="Hover Setting" )
	float HoverSpeed = 300.f;

	bool bCanHover = false;

	bool bIsHovering = false;

	FVector PlaneNormal;
};