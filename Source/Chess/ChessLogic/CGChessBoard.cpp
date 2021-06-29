// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessBoard.h"
#include "ChessGameMode.h"
#include "CGPiece.h"
#include "GameLogic/CGBoardTile.h"
#include "GameLogic/CGChessPlayerController.h"

#define Dbg(x) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT(x));}

// Sets default values
ACGChessBoard::ACGChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bRunConstructionScriptOnDrag = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ACGChessBoard::BeginPlay()
{
	Super::BeginPlay();
	if (UWorld* w = GetWorld())
	{
		AChessGameMode* gameMode = Cast<AChessGameMode>(w->GetAuthGameMode());
		if (gameMode)
		{
			gameMode->Board = this;
		}

		//TODO remove?
		StartGame(Cast<ACGChessPlayerController>(w->GetFirstPlayerController()));
	}
}

// Called every frame
void ACGChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACGChessBoard::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);

	//bool reset = false;
	for (AActor* a : Board)
	{
		if (!a || a->IsActorBeingDestroyed())
		{
			//reset = true;
			Board.Empty();
			break;
		}
	}

	int32 tileCount = Size.X * Size.Y;

	//add or delete to match the tile count
	for (int32 i = Board.Num() - 1; i > tileCount; --i)
	{
		Board[i]->Destroy();
		Board.RemoveAt(i);
	}
	if (UWorld* world = GetWorld())
	{
		for (int32 i = Board.Num(); i < tileCount; ++i)
		{
			FActorSpawnParameters params;
			params.Owner = this;
			ACGBoardTile* newTile = world->SpawnActor<ACGBoardTile>(TileTemplate, params);
			newTile->Board = this;
			Board.Add(newTile);
		}
	}
	else
	{
		//bail
		//TODO: log error
		return;
	}
	for (int32 i = 0; i < tileCount; ++i)
	{
		ACGBoardTile* tile = Board[i];
		if (tile)
		{
			FCGSquareCoord coord{ i / Size.Y,i % Size.Y };
			tile->SetCoord(coord);
			tile->SetActorTransform(CoordToTransform(coord));
		}
	}
	//set up neighbour references
	for (int32 i = 0; i < tileCount; ++i)
	{
		ACGBoardTile* tile = Board[i];

		int otherIdx = i - Size.X;
		tile->Neighbours[ACGBoardTile::NORTH] = (otherIdx < 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i - Size.X + 1;
		tile->Neighbours[ACGBoardTile::NORTH_EAST] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.X == 0) ? nullptr : Board[otherIdx];

		otherIdx = i + 1;
		tile->Neighbours[ACGBoardTile::EAST] = (otherIdx % Size.X == 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i + Size.X + 1;
		tile->Neighbours[ACGBoardTile::SOUTH_EAST] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.X == 0) ? nullptr : Board[otherIdx];

		otherIdx = i + Size.X;
		tile->Neighbours[ACGBoardTile::SOUTH] = (otherIdx < 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i + Size.X - 1;
		tile->Neighbours[ACGBoardTile::SOUTH_WEST] = (otherIdx < 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i - 1;
		tile->Neighbours[ACGBoardTile::WEST] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.X == Size.X-1) ? nullptr : Board[otherIdx];

		otherIdx = i - Size.X - 1;
		tile->Neighbours[ACGBoardTile::NORTH_WEST] = (otherIdx < 0 || otherIdx >= tileCount || otherIdx % Size.X == Size.X - 1) ? nullptr : Board[otherIdx];


	}

}

void ACGChessBoard::Destroyed()
{
	for (ACGBoardTile* t : Board)
	{
		if (t)
		{
			t->Destroy();
		}
	}
	Board.Empty();
	for (ACGPiece* p : Pieces)
	{
		if (p)
		{
			p->Destroy();
		}
	}
	Pieces.Empty();
	Super::Destroyed();
}

void ACGChessBoard::StartGame(ACGChessPlayerController* p1, ACGChessPlayerController* p2)
{

	//TODO: remove
	FenStringToChessPieces(DefaultBoardFen);
}


/*
	default fen:
	rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
*/
bool ACGChessBoard::FenStringToChessPieces(FString fen)
{
	//clear the pieces from the board
	for (ACGPiece* p : Pieces)
	{
		if (p)
		{
			p->Destroy();
		}
	}
	Pieces.Empty();

	int8 x = 0;
	int8 y = 0;
	bool boardFinished = false;
	UWorld* world = GetWorld();
	if (!world)
	{
		return false;
	}
	for (const TCHAR& chr : fen)
	{
		bool charHandled = false;
		if (!boardFinished)
		{
			//is new line
			if (chr == '\\' || chr == '/')
			{
				//x = 0;
				//y += 1;
				charHandled = true;
			}
			//is numeric
			if (chr >= 48 && chr <= 57)
			{
				int8 skip = chr - 48;
				x += skip;
				charHandled = true;
			}
			else
			{
				for (TSubclassOf<class ACGPiece> temp : PieceTemplates)
				{
					if (temp) {
						ACGPiece* def = Cast<ACGPiece>(temp.Get()->GetDefaultObject());
						if (def)
						{
							for (const TCHAR pieceChr : def->GetFenChars())
							{
								if (pieceChr == chr)
								{
									//spawn piece
									FActorSpawnParameters params;
									bool isWhite = TChar<TCHAR>::IsUpper(pieceChr);
									params.Owner = this;
									ACGPiece* newPiece = world->SpawnActor<ACGPiece>(temp, params);
									newPiece->SetMaterial(isWhite ? WhiteMaterial : BlackMaterial);
									newPiece->Board = this;
									newPiece->MoveTo(FCGSquareCoord(x, y), true);
									Pieces.Add(newPiece);

									charHandled = true;
									x += 1;
								}
							}
						}
					}
				}
			}
			if (x >= Size.X)
			{
				x = 0;
				y += 1;
			}
		}
		if (!charHandled)
		{
			return false;
		}
		if (x == Size.X && y == Size.Y)
		{
			boardFinished = true;
			//TODO: remove this!
			return true;
		}
	}
	return true;
}

FTransform ACGChessBoard::CoordToTransform(const FCGSquareCoord& coord) const
{
	float boardHalfSizeX = TileSize.X * .5f * (Size.X - 1);
	float boardHalfSizeY = TileSize.Y * .5f * (Size.Y - 1);

	FTransform tileTransform = GetTransform();
	FVector tileLocation = tileTransform.GetLocation();

	tileLocation.X += coord.X * TileSize.X - boardHalfSizeX;
	tileLocation.Y += coord.Y * TileSize.Y - boardHalfSizeY;
	tileTransform.SetLocation(tileLocation);
	return tileTransform;
}

FCGSquareCoord ACGChessBoard::TransformToCoord(const FTransform& transform)
{
	return LocationToCoord(transform.GetLocation());
}

FCGSquareCoord ACGChessBoard::LocationToCoord(const FVector& location)
{
	float boardHalfSizeX = TileSize.X * .5f * Size.X;// -1);
	float boardHalfSizeY = TileSize.Y * .5f * Size.Y;// -1);

	FVector boardLoc = GetTransform().GetLocation();
	boardLoc.X += boardHalfSizeX;
	boardLoc.Y += boardHalfSizeY;
	boardLoc -= location;
	return FCGSquareCoord(Size.X - boardLoc.X / TileSize.X, Size.Y - boardLoc.Y / TileSize.Y);
}

ACGBoardTile* ACGChessBoard::GetTile(const FCGSquareCoord& coord)
{
	//FCGSquareCoord coord{ i / Size.Y,i % Size.Y };
	ACGBoardTile* ret = Board[coord.X * Size.X + coord.Y];
	ensure(coord == ret->Position);
	return ret;
}

void ACGChessBoard::CoordToLabel(const FCGSquareCoord coord, TCHAR& X, TCHAR& Y)
{
	X = 'A'+coord.X;
	Y = coord.Y + 1;
}

void ACGChessBoard::ApplySkin(ACGChessPlayerController* playerController, int skin)
{

}