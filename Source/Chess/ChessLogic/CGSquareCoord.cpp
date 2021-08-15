#include "CGSquareCoord.h"

bool FCGSquareCoord::operator==(const FCGSquareCoord& iOther) const
{
	return Equals(iOther);
}

bool FCGSquareCoord::operator!=(const FCGSquareCoord& iOther) const
{
	return !Equals(iOther);
}

bool FCGSquareCoord::Equals(const FCGSquareCoord& iOther) const
{
	return X == iOther.X && Y == iOther.Y;
}

/*uint32 FCGSquareCoord::GetTypeHash(const FCGSquareCoord& Coord)
{
	uint32 ret = Coord.X << 8;
	ret += Coord.Y;
	return ret;
}*/