#pragma once

#include "CoreMinimal.h"
#include "SquareCoord.generated.h"

USTRUCT(BlueprintType)
struct FSquareCoord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 X;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 Y;

	FSquareCoord():X(0),Y(0) {};
	FSquareCoord(uint8 x, uint8 y) : X(x), Y(y) {};
	FSquareCoord(const FSquareCoord& Other) : FSquareCoord(Other.X, Other.Y) {};

	bool operator==(const FSquareCoord& Other) const
	{
		return Equals(Other);
	}

	bool Equals(const FSquareCoord& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	//FORCEINLINE
	friend uint32 GetTypeHash(const FSquareCoord& Coord)
	{
		uint32 ret = Coord.X << 8;
		ret += Coord.Y;
		return ret;
	}
};
