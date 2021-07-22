#pragma once

#include "CoreMinimal.h"
#include "CGSquareCoord.generated.h"

USTRUCT(BlueprintType)
struct FCGSquareCoord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Y;

	FCGSquareCoord():X(0),Y(0) {};
	FCGSquareCoord(uint8 x, uint8 y) : X(x), Y(y) {};
	FCGSquareCoord(int32 x, int32 y) : X(x), Y(y) {};
	FCGSquareCoord(float x, float y) : X(x), Y(y) {};
	FCGSquareCoord(const FCGSquareCoord& Other) : FCGSquareCoord(Other.X, Other.Y) {};

	bool operator==(const FCGSquareCoord& Other) const
	{
		return Equals(Other);
	}

	bool operator!=(const FCGSquareCoord& Other) const
	{
		return !Equals(Other);
	}

	bool Equals(const FCGSquareCoord& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FCGSquareCoord& Coord)
	{
		uint32 ret = Coord.X << 8;
		ret += Coord.Y;
		return ret;
	}
};
