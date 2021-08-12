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
	DRAW_50_MOVE	UMETA(DisplayName = "Draw by 50 move rule!"),
	DRAW_3_REP		UMETA(DisplayName = "Draw by threefold repetition!"),
	DRAW_5_REP		UMETA(DisplayName = "Draw by fivefold repetition!"),
	DISCONNECT		UMETA(DisplayName = "Disconnected."),
};