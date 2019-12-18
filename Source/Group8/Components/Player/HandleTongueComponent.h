#pragma once
#include "Components/ActorComponent.h"
#include "../../Enums/AbilityStateEnum.h"
#include "HandleTongueComponent.generated.h"

UCLASS()
class GROUP8_API UHandleTongueComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UHandleTongueComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;

	UFUNCTION()
	void OnBeginSphereOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult );

	void UseTongue();

	//could be BlueprintNative/ImplementableEvent
	/*UFUNCTION()
		void OnInteract();*/

	//I assume oskar is gonna be working with this
	UFUNCTION(BlueprintImplementableEvent, Category = "Grapple")
	void OnGrapple();

	/*UFUNCTION()
	void AbilityStateChanged( EAbilityState NewState );*/

	void SetAbilityActive();

	
	UPROPERTY(EditDefaultsOnly, Category = "Is Active")
	bool bTongueActive = false;

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Tongue Collider")
	class USphereComponent* SphereCollider;


};