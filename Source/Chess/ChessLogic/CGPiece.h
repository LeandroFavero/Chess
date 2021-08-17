// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLogic/CGUndo.h"
#include "ChessLogic/CGSquareCoord.h"
#include "CGPiece.generated.h"

class UCGAnimInstance;
class ACGChessPlayerController;
class ACGChessBoard;
class ACGTile;

struct EPieceFlags
{
	enum EPieceFlagsValues
	{
		IsBlack = 1 << 0,			//bit0
		Captured = 1 << 2,			//bit2
		Moved = 1 << 3,				//bit3
		EnPassantCaptured = 1 << 4,	//bit4
		DummyCaptured = 1 << 5,		//bit5
		//PromotionInProgress = 1 << 6//bit6
	};
};

UCLASS()
class CHESS_API ACGPiece : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	TSubclassOf<class UCGAnimInstance> AnimTemplate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Chess setup")
	UStaticMeshComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chess setup")
	ACGChessPlayerController* OwningPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = SnapToPlace, Category = "Chess setup")
	FCGSquareCoord Position;

	UPROPERTY(Replicated)
	ACGChessBoard* Board;

	UPROPERTY(Replicated)
	ACGTile* Tile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Chess setup")
	uint8 Flags = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chess setup")
	int Value;

protected:
	void PostInitializeComponents() override;

public:	

	ACGPiece();

	ACGPiece(ACGChessBoard* iBoard, uint8 iFlags = 0);

	virtual void Destroyed() override;

	UFUNCTION(BlueprintPure, Category = "Chess FEN")
	virtual const FString GetUnicode() const { return ""; }

	virtual const bool IsFenMatches(const TCHAR& iChr) const { return false; }

	UFUNCTION(BlueprintPure, Category = "Chess FEN")
	virtual const FString GetFenChar() const { return ""; }

	UFUNCTION(BlueprintPure, Category = "Chess setup")
	virtual const bool IsValidForPromotion() const { return false; }

	virtual void RefreshMaterial();

	UFUNCTION(BlueprintCallable, Category = "Chess setup")
	virtual void SetColor(bool IsWhite);

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void Grab(bool IsGrabbed);

	UFUNCTION()
	virtual void SnapToPlace();
	
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, Category = "Chess")
	virtual void ClientSnapToPlace();
	virtual void ClientSnapToPlace_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual void MoveToTile(ACGTile* TargetTile);
	virtual void MoveToTileInternal(ACGTile* iTile, FCGUndo& oUndo, bool iEvents = true);

	UFUNCTION(BlueprintPure, Category = "Chess")
	virtual const bool IsBlack() const { return (Flags & EPieceFlags::IsBlack) == EPieceFlags::IsBlack; }

	UFUNCTION(BlueprintPure, Category = "Chess")
	virtual const bool IsWhite() const { return (Flags & EPieceFlags::IsBlack) != EPieceFlags::IsBlack; }

	UFUNCTION(BlueprintPure, Category = "Chess")
	virtual const bool IsMoved() const { return (Flags & EPieceFlags::Moved) == EPieceFlags::Moved; }

	UFUNCTION(BlueprintCallable, Category = "Chess")
	virtual const bool IsCaptured() const;

	virtual TSet<ACGTile*> GetAvailableMoves();

	virtual bool HasAvailableMoves();

	virtual void FillAttackMap();

	virtual void Capture(bool iAddToCaptured = true);

	virtual void UnCapture();

	virtual ACGChessPlayerController* GetCGController();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
