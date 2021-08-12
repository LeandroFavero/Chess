#pragma once

#include "CoreMinimal.h"
#include "ChessLogic/CGSquareCoord.h"
#include "CGUndo.generated.h"

class ACGPiece;
class ACGTile;

USTRUCT(BlueprintType)
struct CHESS_API FCGUndo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chess")
	int MoveNumber { 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chess")
	int FiftyMoveCounter { 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chess")
	int FenMoveNumber { 0 };

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Piece { nullptr };

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Promotion { nullptr };

	UPROPERTY(VisibleAnywhere)
	ACGPiece* Capture { nullptr };

	UPROPERTY(VisibleAnywhere)
	ACGPiece* CastleRook { nullptr };

	UPROPERTY(VisibleAnywhere)
	ACGTile* CastleRookTile { nullptr };

	UPROPERTY(VisibleAnywhere)
	ACGTile* From { nullptr };

	UPROPERTY(VisibleAnywhere)
	ACGTile* To { nullptr };

	UPROPERTY(VisibleAnywhere)
	uint8 Flags { 0 };

	UPROPERTY(VisibleAnywhere)
	bool SimpleNotation { true };

	UPROPERTY(VisibleAnywhere)
	bool Check { false };

	UPROPERTY(VisibleAnywhere)
	bool LastMoveIsBlack { true };

	UPROPERTY(VisibleAnywhere)
	bool Imported { false };

	UPROPERTY(VisibleAnywhere)
	FString Fen;
	
	FCGUndo(const int iMoveNumber, const int iHalfMovesSincePawnMove, const int iFenMoveNumber)
		:MoveNumber(iMoveNumber),
		FiftyMoveCounter(iHalfMovesSincePawnMove),
		FenMoveNumber(iFenMoveNumber) {}
	FCGUndo() {}
};