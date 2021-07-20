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
	FCGSquareCoord Size {8, 8};

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FVector2D TileSize {200, 200};

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FString DefaultBoardFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	UPROPERTY(EditAnywhere, Category = "Chess setup")
	bool EnforceMoveOrder{ true };

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

	ACGChessBoard();

	UPROPERTY(ReplicatedUsing = UndoNotify, BlueprintReadOnly, Category = "Chess")
	TArray<FCGUndo> Undos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	UCGCapturedPieces* CapturedWhite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	UCGCapturedPieces* CapturedBlack;

public:	

	virtual void OnConstruction(const FTransform& Transform) override;

	void Destroyed() override;

	//https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual bool FenStringToChessPieces(const FString& Fen);

	virtual bool CheckTest() { return false; };

	virtual void RebuildAttackMap(bool IsBlack);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual bool NextMoveIsBlack() const;

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual bool ReadyForNextMove() const;

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void StartGame(const FString& Fen, ACGChessPlayerController* P1, ACGChessPlayerController* P2 = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual FTransform CoordToTransform(const FCGSquareCoord& Coord) const;

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual FCGSquareCoord TransformToCoord(const FTransform& Transform);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual FCGSquareCoord LocationToCoord(const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual bool HasValidMove(bool IsBlack);

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual bool GameOverCheck(bool IsBlack);

	virtual void RefreshPieceColors();
	virtual ACGTile* GetTile(const FCGSquareCoord& Coord);
	virtual FCGUndo& CreateUndo();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void UndoTo(int MoveNum);
	virtual void UndoInternal(FCGUndo& Undo);

	//TODO... Let's just cut this for now....
	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void RequestUndoTo(int MoveNum);

	UFUNCTION()
	virtual void UndoNotify();

	UFUNCTION()
	virtual void PiecesNotify();

	static void CoordToLabel(const FCGSquareCoord Coord, TCHAR& X, TCHAR& Y);

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