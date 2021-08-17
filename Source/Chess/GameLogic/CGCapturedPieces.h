#pragma once

class ACGPiece;

#include "CoreMinimal.h"
#include "ChessLogic/CGSquareCoord.h"
#include "CGCapturedPieces.generated.h"

UCLASS()
class UCGCapturedPieces :public USceneComponent
{
	GENERATED_BODY()

	UPROPERTY(ReplicatedUsing = CapturedNotify)
	TArray<ACGPiece*> Pieces;

public:
	UPROPERTY(EditAnywhere, Category = "Chess setup")
	FVector Offset {0, 100, 0};

	UCGCapturedPieces();

	UFUNCTION(BlueprintCallable)
	void RefreshPositions();

	UFUNCTION()
	void CapturedNotify();

	void Add(ACGPiece* iPiece);

	void Remove(ACGPiece* iPiece);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

