#pragma once

#include "AbilityBerryEnum.generated.h"

UENUM(BlueprintType)
enum class EAbilityBerry : uint8
{
	AB_None,
	AB_Hover,
	AB_Dash,
	AB_Jump,
	AB_Tongue
};
