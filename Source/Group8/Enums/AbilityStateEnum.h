#pragma once

#include "AbilityStateEnum.generated.h"

UENUM( BlueprintType )
enum class EAbilityState : uint8
{
	AS_None,
	AS_Walking,
	AS_Jump,
	AS_Falling,
	AS_Hover,
	AS_Dash,
	AS_AirDash,
	AS_ShortDash,
	AS_Push,
	AS_TongueInteracting,
	AS_TongueGrappling

};
