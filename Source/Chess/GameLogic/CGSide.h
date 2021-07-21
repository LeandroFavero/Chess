#pragma once

#include "CoreMinimal.h"
#include "CGSide.generated.h"

UENUM(BlueprintType)
enum ESide
{
	WHITE			UMETA(DisplayName = "White"),
	BLACK			UMETA(DisplayName = "Black"),
	NONE			UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum EGameResult
{
	NOT_FINISHED	UMETA(DisplayName = ""),
	BLACK_WINS		UMETA(DisplayName = "Black wins!"),
	WHITE_WINS		UMETA(DisplayName = "White wins!"),
	DRAW			UMETA(DisplayName = "Draw!"),
	DISCONNECT		UMETA(DisplayName = "Disconnected.")
};