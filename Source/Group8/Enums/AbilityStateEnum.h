#pragma once

#include "AbilityStateEnum.generated.h"

//TO DO
//Removed unused types
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
};
