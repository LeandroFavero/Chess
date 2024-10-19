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

	explicit FCGSquareCoord():X(0),Y(0) {};
	explicit FCGSquareCoord(uint8 x, uint8 y) : X(x), Y(y) {};
	explicit FCGSquareCoord(int32 x, int32 y) : X(x), Y(y) {};
	explicit FCGSquareCoord(float x, float y) : X(x), Y(y) {};
	explicit FCGSquareCoord(double x, double y) : X(x), Y(y) {};
	explicit FCGSquareCoord(const FCGSquareCoord& Other) : FCGSquareCoord(Other.X, Other.Y) {};

	bool operator==(const FCGSquareCoord& iOther) const;

	bool operator!=(const FCGSquareCoord& iOther) const;

	bool Equals(const FCGSquareCoord& iOther) const;

	//FORCEINLINE friend uint32 GetTypeHash(const FCGSquareCoord& Coord);
};
