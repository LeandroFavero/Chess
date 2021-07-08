#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGSquareCoord.h"
#include "CGUndo.generated.h"

class ACGPiece;
class ACGBoardTile;

/**
 *
 */
USTRUCT(BlueprintType)
struct CHESS_API FCGUndo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chess")
	int MoveNumber {0};

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Piece {nullptr};

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Promotion {nullptr};

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Capture {nullptr};

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Castle {nullptr};

	UPROPERTY(VisibleAnywhere)
	ACGBoardTile* From {nullptr};

	UPROPERTY(VisibleAnywhere)
	ACGBoardTile* To {nullptr};

	UPROPERTY(VisibleAnywhere)
	uint8 Flags {0};
	
	FCGUndo(int pMoveNumber):MoveNumber(pMoveNumber) {}
	FCGUndo() {}

	//virtual bool operator == (const FCGUndo& other)
	//{
	//	return MoveNumber == other.MoveNumber /* && Piece == other.Piece*/ && From == other.From && To == other.To && Flags == other.Flags;
	//}
	/*FORCEINLINE	friend uint32 GetTypeHash(const FCGUndo& Undo)
	{
		uint32 ret = static_cast<uint32>(Undo.Flags) << 24;
		uint32 ret = Coord.X << 8;
		ret += Coord.Y;
		return ret;
	}*/
};