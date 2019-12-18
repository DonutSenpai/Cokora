#pragma once
#include "GameFramework/Actor.h"
#include "WorldInteractableObject.generated.h"

UCLASS()
class GROUP8_API AWorldInteractableObject : public AActor
{
	GENERATED_BODY()

public:

	AWorldInteractableObject();
	UFUNCTION()
	void OnBeginBoxOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult );

	UFUNCTION()
	void	OnBeginSphereOverlap( class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult );

	UFUNCTION(BlueprintImplementableEvent)
	void OnOverlap();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Box Collider")
	class UBoxComponent* BoxCollider = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Sphere Collider")
	class USphereComponent* SphereCollider = nullptr;

};