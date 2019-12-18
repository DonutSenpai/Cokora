#pragma once
#include "GameFramework/Actor.h"
#include "../../../Enums/AbilityBerryEnum.h"
#include "AbilityBerryBase.generated.h"

UCLASS()
class GROUP8_API AAbilityBerryBase : public AActor
{
	GENERATED_BODY()
public:

	AAbilityBerryBase();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginSphereOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void OnPickUp( EAbilityBerry BerryType );

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	class USphereComponent* SphereCollider = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "BerryType")
	EAbilityBerry BerryType = EAbilityBerry::AB_None;
};