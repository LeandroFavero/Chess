// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/IntPoint.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessLogic/CGSquareCoord.h"
#include "GameLogic/CGUndo.h"
#include "CGChessBoard.generated.h"

class ACGChessPlayerController;
class UCGCapturedPieces;
class ACGTile;
class ACGKing;
class ACGPiece;

UCLASS(Blueprintable)
class CHESS_API ACGChessBoard : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Chess setup")
	TSubclassOf<class ACGTile> TileTemplate;

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FCGSquareCoord Size { 8, 8 };

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FVector2D TileSize { 200, 200 };

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FString DefaultBoardFen = TEXT("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	bool EnforceMoveOrder { true };

	UPROPERTY(Replicated)
	TArray<ACGTile*> Board;
	
	UPROPERTY(ReplicatedUsing=PiecesNotify)
	TArray<ACGPiece*> Pieces;

	UPROPERTY(Replicated)
	ACGKing* WhiteKing;

	UPROPERTY(Replicated)
	ACGKing* BlackKing;

	UMaterialInstance* umi;

	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY(ReplicatedUsing = UndoNotify, BlueprintReadOnly, Category = "Chess")
	TArray<FCGUndo> Undos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	UCGCapturedPieces* CapturedWhite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	UCGCapturedPieces* CapturedBlack;

	ACGChessBoard();

	virtual void OnConstruction(const FTransform& Transform) override;

	void Destroyed() override;

	//https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	bool FenStringToChessPieces(const FString& Fen);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	FString PiecesToFen(bool iIsForUndo = false) const;

	virtual void RebuildAttackMap(bool iIsBlack);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual bool IsNextMoveBlack() const;

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual bool IsReadyForNextMove() const;

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void StartGame(const FString& Fen, ACGChessPlayerController* P1, ACGChessPlayerController* P2 = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual FTransform CoordToTransform(const FCGSquareCoord& Coord) const;

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual FCGSquareCoord TransformToCoord(const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual FCGSquareCoord LocationToCoord(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual bool GameOverCheck();

	virtual void RefreshPieceColors();
	virtual ACGTile* GetTile(const FCGSquareCoord& iCoord) const;
	virtual FCGUndo& CreateUndo();
	virtual const FCGUndo* GetLastUndo() const;

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void UndoTo(const int MoveNum);
	virtual void UndoInternal(const FCGUndo& oUndo);

	//TODO... Let's just cut this for now....
	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void RequestUndoTo(int MoveNum);

	UFUNCTION()
	virtual void UndoNotify();

	UFUNCTION()
	virtual void PiecesNotify();

	//static void CoordToLabel(const FCGSquareCoord& iCoord, TCHAR& oX, TCHAR& oY);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
};

enum FenField
{
	PIECE_PLACEMENT = 0,
	NEXT_MOVE = 1,
	CASTLING_AVAILABILITY = 2,
	EN_PASSANT_TARGET_SQUARE = 3,
	HALFMOVE_CLOCK = 4,
	FULLMOVE_NUMBER = 5
};