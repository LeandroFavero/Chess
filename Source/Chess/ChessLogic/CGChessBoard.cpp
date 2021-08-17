// Fill out your copyright notice in the Description page of Project Settings.

#include "CGChessBoard.h"
#include "GameLogic/CGGameState.h"
#include "GameLogic/CGGameMode.h"
#include "CGPawn.h"
#include "CGRook.h"
#include "CGKing.h"
#include "ChessLogic/CGTile.h"
#include "GameLogic/CGChessPlayerController.h"
#include "GameLogic/CGCapturedPieces.h"
#include "UI/CGHUD.h"
#include "Misc/Char.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/CGBPUtils.h"

#define Dbg(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT(x), __VA_ARGS__));}

ACGChessBoard::ACGChessBoard()
{
#if WITH_EDITOR
	bRunConstructionScriptOnDrag = false;
#endif

	PrimaryActorTick.bCanEverTick = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	bReplicates = true;
	bOnlyRelevantToOwner = false;
}

void ACGChessBoard::OnConstruction(const FTransform& iTransform)
{
	Super::OnConstruction(iTransform);

	for (AActor* a : Board)
	{
		if (!a || a->IsActorBeingDestroyed())
		{
			//reset = true;
			Board.Empty();
			break;
		}
	}
	if (!TileTemplate)
	{
		return;
	}
	
	int32 tileCount = Size.X * Size.Y;

	//add or delete to match the tile count
	for (int32 i = Board.Num() - 1; i > tileCount; --i)
	{
		Board[i]->Destroy();
		Board.RemoveAt(i);
	}
	if (UWorld* w = GetWorld())
	{
		for (int32 i = Board.Num(); i < tileCount; ++i)
		{
			FActorSpawnParameters params;
			params.Owner = this;
			ACGTile* newTile = w->SpawnActor<ACGTile>(TileTemplate, params);
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
		ACGTile* tile = Board[i];
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
		ACGTile* tile = Board[i];

		int otherIdx = i + 1;
		tile->Neighbours[static_cast<int>(EDir::NORTH)] = ((otherIdx % Size.Y) == 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::NORTH_EAST)] = ((otherIdx % Size.Y) == 0 || otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::EAST)] = (otherIdx >= tileCount) ? nullptr : Board[otherIdx];

		otherIdx = i - 1 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::SOUTH_EAST)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];

		otherIdx = i - 1;
		tile->Neighbours[static_cast<int>(EDir::SOUTH)] = (otherIdx < 0 || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];

		otherIdx = i - 1 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::SOUTH_WEST)] = (otherIdx < 0 || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];

		otherIdx = i - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::WEST)] = (otherIdx < 0) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::NORTH_WEST)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == 0) ? nullptr : Board[otherIdx];

		//knight moves clockwise
		otherIdx = i + 2 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT1)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == 0 || (otherIdx % Size.Y) == 1) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 + (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT2)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == 0) ? nullptr : Board[otherIdx];

		otherIdx = i - 1 + (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT3)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];

		otherIdx = i - 2 + Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT4)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == (Size.Y - 2) || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];

		otherIdx = i - 2 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT5)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == (Size.Y - 2) || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];
		
		otherIdx = i - 1 - (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT6)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == (Size.Y - 1)) ? nullptr : Board[otherIdx];

		otherIdx = i + 1 - (Size.Y * 2);
		tile->Neighbours[static_cast<int>(EDir::KNIGHT7)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == 0) ? nullptr : Board[otherIdx];

		otherIdx = i + 2 - Size.Y;
		tile->Neighbours[static_cast<int>(EDir::KNIGHT8)] = (otherIdx < 0 || otherIdx >= tileCount || (otherIdx % Size.Y) == 0 || (otherIdx % Size.Y) == 1) ? nullptr : Board[otherIdx];

	}
}

void ACGChessBoard::Destroyed()
{
	for (ACGTile* t : Board)
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
	Undos.Empty();
	Super::Destroyed();
}

void ACGChessBoard::StartGame(const FString& iFen, ACGChessPlayerController* iP1, ACGChessPlayerController* iP2)
{
	if (HasAuthority())
	{
		FenStringToChessPieces(iFen.IsEmpty() ? DefaultBoardFen : iFen);
		//TODO: prefered side vs chess engine!
		if (iP1)
		{
			if (iP1->PreferredSide == ESide::NONE || (iP2 && iP2->PreferredSide == iP1->PreferredSide))
			{
				//randomize?
				iP1->bIsBlack = static_cast<bool>(FMath::RandRange(0, 1));
				if (iP2)
				{
					iP2->bIsBlack = !iP1->bIsBlack;
				}
			}
			else
			{
				//give them what they want
				if (iP1->PreferredSide == ESide::NONE && iP2)
				{
					iP1->bIsBlack = iP2->PreferredSide == ESide::WHITE;
				}
				else
				{
					iP1->bIsBlack = iP1->PreferredSide == ESide::BLACK;
				}
				if (iP2)
				{
					iP2->bIsBlack = iP1->bIsBlack;
				}
			}
		}
	}
}

/*
	default Fen:
	rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
*/
bool ACGChessBoard::FenStringToChessPieces(const FString& iFen)
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
	Undos.Empty();

	int8 x = 0;
	int8 y = Size.Y - 1;
	bool malformed { false };
	UWorld* world = GetWorld();
	if (!world)
	{
		return false;
	}
	FCGUndo* undo = nullptr;
	int field = 0;
	auto it = iFen.CreateConstIterator();
	for ( ;field == FenField::PIECE_PLACEMENT && it && *it != ' '; ++it)
	{
		const TCHAR& chr = *it;

		if (x >= Size.X)
		{
			x = 0;
			y -= 1;
			if (y < 0)
			{
				malformed = true;
				break;
			}
		}

		//is new line
		if (chr == '\\' || chr == '/')
		{
			if (x != 0)
			{
				malformed = true;
			}
			continue;
		}
		//is numeric
		if (chr >= '0' && chr <= '9')
		{
			int8 skip = chr - '0';
			x += skip;
			continue;
		}
		else
		{
			if (ACGGameState* gameState = world->GetGameState<ACGGameState>())
			{
				FCGUndo dummyUndo;//we don't want to undo the initial piece spawns!
				for (TSubclassOf<class ACGPiece>& temp : gameState->PieceTemplates)
				{
					if (temp)
					{
						if (ACGPiece* def = Cast<ACGPiece>(temp.Get()->GetDefaultObject()))
						{
							if (def->IsFenMatches(chr))
							{
								//spawn piece
								FActorSpawnParameters params;
								bool isWhite = TChar<TCHAR>::IsUpper(chr);
								params.Owner = this;
								ACGPiece* newPiece = world->SpawnActor<ACGPiece>(temp, params);
								newPiece->SetColor(isWhite);
								newPiece->Board = this;
								newPiece->MoveToTileInternal(GetTile({ x,y }), dummyUndo, false);
								Pieces.Add(newPiece);

								//ptr to the kings
								if (newPiece->IsA(ACGKing::StaticClass()))
								{
									if (newPiece->IsBlack())
									{
										BlackKing = Cast<ACGKing>(newPiece);
									}
									else
									{
										WhiteKing = Cast<ACGKing>(newPiece);
									}
								}
								x += 1;
								goto continueNextPiece;
							}
						}
					}
				}
			}
		}
		//the only way to get here is to find an unexpected character
		malformed = true;
	continueNextPiece:;
	}

	field += 1;
	if (it)
	{
		++it;
	}
	for (; field == FenField::NEXT_MOVE && it && *it != ' '; ++it)
	{
		const TCHAR& chr = *it;
		if (chr == 'w' || chr == 'W')
		{
			//there's nothing to do, the first move belongs to white already
		}
		else if (chr == 'b' || chr == 'B')
		{
			//add dummy undo token with a white move
			if (!undo)
			{
				undo = &CreateUndo();
			}
			undo->Imported = true;
			undo->LastMoveIsBlack = false;

		}
		else
		{
			malformed = true;
		}
	}
	field += 1;
	if (it)
	{
		++it;
	}
	FString tmp("KQkq",0);
	for (; field == FenField::CASTLING_AVAILABILITY && it; ++it)
	{
		const TCHAR& chr = *it;
		if (chr == ' ')
		{
			//we're done!
			field += 1;
			if (BlackKing && WhiteKing) 
			{
				//flag the remaining pieces moved, so they can't castle
				for (const TCHAR& chr2 : tmp)
				{
					if (chr2 == 'K' || chr2 == 'Q' || chr2 == 'k' || chr2 == 'q')
					{
						for (ACGTile* t = (TChar<TCHAR>::IsLower(chr2) ? BlackKing : WhiteKing)->Tile; t; t = t->Neighbours[(chr2 == 'K' || chr2 == 'k') ? EDir::WEST : EDir::EAST])
						{
							if (t->OccupiedBy && t->OccupiedBy->IsA(ACGRook::StaticClass()))
							{
								t->OccupiedBy->Flags |= EPieceFlags::Moved;
								goto continueNextChar;
							}
						}
					}
					else
					{
						malformed = true;
					}
				continueNextChar:;
				}
			}
		}
		else
		{
			//since rooks can castle by default, we have to find what's missing from the string
			int idx{ 0 };
			if (tmp.FindChar(chr, idx))
			{
				tmp.RemoveAt(idx);
			}
			else if(chr != '-')
			{
				malformed = true;
			}
		}
	}

	tmp.Empty(4);
	for (; field == FenField::EN_PASSANT_TARGET_SQUARE && it && *it != ' '; ++it)
	{
		//the square that was skipped with a pawn double opener
		if (*it != '-')
		{
			for (; it && *it != ' '; ++it)
			{
				const TCHAR& chr = *it;
				tmp.AppendChar(chr);
			}
			//find the pawn that did the double open and add dummy undo token
			if (tmp.Len() == 2)
			{
				tmp = tmp.ToLower();
				int tileX = tmp[0] - 'a';
				int tileY = tmp[1] - '1';
				bool isBlack = tileY > (Size.Y / 2);
				tileY = isBlack ? tileY - 1 : tileY + 1;
				ACGTile* t = GetTile({ tileX, tileY });
				if (t && t->OccupiedBy)
				{
					if (!undo)
					{
						undo = &CreateUndo();
					}
					undo->Imported = true;
					undo->Piece = t->OccupiedBy;
					undo->To = t;
					for (int distance = 0; distance < 2; ++distance)
					{
						if (t)
						{
							t = t->Neighbours[isBlack ? EDir::NORTH : EDir::SOUTH];
						}
					}
					undo->From = t;
				}
			}
			else
			{
				malformed = true;
			}
		}
		field += 1;
	}
	tmp.Empty(4);
	if (it)
	{
		++it;
	}
	for (; field == FenField::HALFMOVE_CLOCK && it && *it != ' '; ++it)
	{
		tmp.AppendChar(*it);
	}
	int moveNum = FCString::Atoi(*tmp);
	if (moveNum != DEFAULT_FEN_HALF_MOVE)
	{
		if (!undo)
		{
			undo = &CreateUndo();
		}
		undo->Imported = true;
		undo->FiftyMoveCounter = moveNum;
	}
	field += 1;
	if (it)
	{
		++it;
	}
	tmp.Empty(4);
	for (; field == FenField::FULLMOVE_NUMBER && it && *it != ' '; ++it)
	{
		tmp.AppendChar(*it);
	}
	moveNum = FCString::Atoi(*tmp);
	if (moveNum != DEFAULT_FEN_MOVE_NUMBER)
	{
		if (!undo)
		{
			undo = &CreateUndo();
		}
		undo->Imported = true;
		undo->FenMoveNumber = moveNum;
	}
	for (ACGPiece* p : Pieces)
	{
		p->ClientSnapToPlace();
	}
	if (!WhiteKing || !BlackKing)
	{
		malformed = true;
	}
	if (malformed)
	{
		StartingFen = PiecesToFen();
	}
	else
	{
		StartingFen = iFen;
	}
	//update moves on listen server
	if (Undos.Num() > 0 && UCGBPUtils::IsLocalUpdateRequired(this))
	{
		UndoNotify();
	}
	return !malformed;
}

FString ACGChessBoard::PiecesToFen(bool iIsForUndo)
{
	ACGPiece* whiteKSRook = nullptr, * whiteQSRook = nullptr,
		* blackKSRook = nullptr, * blackQSRook = nullptr;

	FString ret;
	for (int y = Size.Y - 1; y >= 0; --y)
	{
		int empties = 0;
		for (int x = 0; x < Size.X; ++x)
		{
			const FCGSquareCoord c(x, y);
			if (ACGTile* tile = GetTile(c))
			{
				ACGPiece* p = tile->OccupiedBy;
				if (p)
				{
					if (empties > 0)
					{
						ret.AppendInt(empties);
						empties = 0;
					}
					ret.Append(p->GetFenChar());
					//store reference if it's a casting capable rook
					if (p->IsA(ACGRook::StaticClass()) && !p->IsMoved())
					{
						if (p->IsWhite())
						{
							if (p->Position.X < WhiteKing->Position.X)
							{
								whiteQSRook = p;
							}
							else
							{
								whiteKSRook = p;
							}
						}
						else
						{
							if (p->Position.X < BlackKing->Position.X)
							{
								blackQSRook = p;
							}
							else
							{
								blackKSRook = p;
							}
						}
					}
				}
				else
				{
					empties += 1;
				}
			}
		}
		if (empties > 0)
		{
			ret.AppendInt(empties);
		}
		if (y != 0)
		{
			ret.AppendChar('/');
		}
	}
	if (IsNextMoveBlack())
	{
		ret.Append(" b ");
	}
	else
	{
		ret.Append(" w ");
	}
	//available castlings
	FString castlings;
	if (!WhiteKing->IsMoved())
	{
		if (whiteKSRook)
		{
			castlings.AppendChar('K');
		}
		if (whiteQSRook)
		{
			castlings.AppendChar('Q');
		}
	}
	if (!BlackKing->IsMoved())
	{
		if (blackKSRook)
		{
			castlings.AppendChar('k');
		}
		if (blackQSRook)
		{
			castlings.AppendChar('q');
		}
	}
	ret.Append(castlings.IsEmpty() ? "-" : castlings);
	//en passant tile
	ret.AppendChar(' ');
	const FCGUndo* last = GetLastUndo();
	if (last && last->Piece && last->Piece->IsA(ACGPawn::StaticClass()) && 
		last->From && last->To && FMath::Abs(last->From->Position.Y - last->To->Position.Y) == 2)
	{
		ret.AppendChar('a' + last->From->Position.X);
		if (last->From->Neighbours[NORTH] == last->To->Neighbours[SOUTH])
		{
			ret.AppendChar('1' + last->From->Neighbours[NORTH]->Position.Y);
		}
		else if (last->From->Neighbours[SOUTH] == last->To->Neighbours[NORTH])
		{
			ret.AppendChar('1' + last->From->Neighbours[SOUTH]->Position.Y);
		}
	}
	else
	{
		ret.Append("-");
	}
	if (!iIsForUndo)
	{
		ret.AppendChar(' ');
		ret.AppendInt(last ? last->FiftyMoveCounter : 0);
		ret.AppendChar(' ');
		ret.AppendInt(last ? last->FenMoveNumber : 1);
	}
	return ret;
}

FTransform ACGChessBoard::CoordToTransform(const FCGSquareCoord& iCoord) const
{
	float boardHalfSizeX = TileSize.X * .5f * (Size.X - 1);
	float boardHalfSizeY = TileSize.Y * .5f * (Size.Y - 1);

	FTransform tileTransform = GetTransform();
	FVector tileLocation = tileTransform.GetLocation();

	tileLocation.X += iCoord.X * TileSize.X - boardHalfSizeX;
	tileLocation.Y += (Size.Y-1-iCoord.Y) * TileSize.Y - boardHalfSizeY;
	tileTransform.SetLocation(tileLocation);
	return tileTransform;
}

FCGSquareCoord ACGChessBoard::TransformToCoord(const FTransform& iTransform)
{
	return LocationToCoord(iTransform.GetLocation());
}

FCGSquareCoord ACGChessBoard::LocationToCoord(const FVector& iLocation)
{
	float boardHalfSizeX = TileSize.X * .5f * Size.X;// -1);
	float boardHalfSizeY = TileSize.Y * .5f * Size.Y;// -1);

	FVector boardLoc = GetTransform().GetLocation();
	boardLoc.X += boardHalfSizeX;
	boardLoc.Y += boardHalfSizeY;
	boardLoc -= iLocation;
	return FCGSquareCoord(Size.X - boardLoc.X / TileSize.X, boardLoc.Y / TileSize.Y);
}

bool ACGChessBoard::GameOverCheck(bool iClaimDraw)
{
	bool isBlack = IsNextMoveBlack();
	if (BlackKing && WhiteKing)
	{
		//50move and threefold repetition tests
		//refresh fen on the last undo
		if (FCGUndo* undo = GetLastUndo())
		{
			UpdateAllPcDrawAvailable(false);
			undo->Fen = PiecesToFen(true);
			EGameResult res = DrawTests(*undo);
			if (res != NOT_FINISHED)
			{
				if (res == DRAW_5_REP || iClaimDraw)
				{
					EndGame(res);
				}
				else
				{
					UpdateAllPcDrawAvailable(true);
				}
			}
		}
		if (iClaimDraw)
		{
			if (Undos.Num() > 1)
			{
				EGameResult res = DrawTests(Undos.Last(1));
				if (res != NOT_FINISHED)
				{
					EndGame(res);
				}
			}
		}
		//check for checkmate
		ACGKing* k = isBlack ? BlackKing : WhiteKing;
		if (k->IsCaptured())//Should not be possible, but end the game if the king is somehow captured
		{
			EndGame(isBlack ? WHITE_WINS : BLACK_WINS);
			return true;
		}
		//find a piece with valid move
		ACGPiece** found = Pieces.FindByPredicate([isBlack](ACGPiece* p) {
			return p && !p->IsCaptured() && p->IsBlack() == isBlack && p->HasAvailableMoves();
		});

		if (found == nullptr)//no valid move, it's game over!
		{
			//checkmate?
			if (k->IsInCheck())
			{
				EndGame(isBlack ? WHITE_WINS : BLACK_WINS);
			}
			else
			{
				EndGame(DRAW);
			}
			return true;
		}
	}
	return false;
}

void ACGChessBoard::EndGame(const EGameResult iResult)
{
	if (UWorld* w = GetWorld())
	{
		ACGGameMode* mode = w->GetAuthGameMode<ACGGameMode>();
		ACGGameState* state = w->GetGameState<ACGGameState>();
		if (mode && state)
		{
			mode->EndMatch();
			state->GameResult = iResult;
			if (UCGBPUtils::IsLocalUpdateRequired(this))
			{
				state->ResultNotify();
			}
		}
	}
}

EGameResult ACGChessBoard::DrawTests(const FCGUndo& iUndo) const
{
	//https://en.wikipedia.org/wiki/Fifty-move_rule
	if (iUndo.FiftyMoveCounter >= 100)
	{
		return DRAW_50_MOVE;
	}
	//https://en.wikipedia.org/wiki/Threefold_repetition
	int repeatCount = 0;
	for (const FCGUndo& u : Undos)
	{
		if (u.Fen.Equals(iUndo.Fen))
		{
			repeatCount += 1;
		}
	}
	if (repeatCount >= 5)
	{
		return DRAW_5_REP;
	}
	else if (repeatCount >= 3)
	{
		return DRAW_3_REP;
	}
	return NOT_FINISHED;
}

void ACGChessBoard::UpdateAllPcDrawAvailable(const bool iIsAvailable)
{
	for (auto it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		if (ACGChessPlayerController* pc = Cast<ACGChessPlayerController>(*it))
		{
			if (pc->bIsDrawClaimable != iIsAvailable)
			{
				pc->bIsDrawClaimable = iIsAvailable;
				if (pc->IsLocalPlayerController() && UCGBPUtils::IsLocalUpdateRequired(this))
				{
					pc->DrawClaimableChanged();
				}
			}
		}
	}
}

void ACGChessBoard::RefreshPieceColors()
{
	for (ACGPiece * p : Pieces)
	{
		if (p)
		{
			p->RefreshMaterial();
		}
	}
}

ACGTile* ACGChessBoard::GetTile(const FCGSquareCoord& iCoord) const
{
	ACGTile* ret = Board[iCoord.X * Size.X + iCoord.Y];
	ensure(iCoord == ret->Position);
	return ret;
}

FCGUndo& ACGChessBoard::CreateUndo()
{
	if (const FCGUndo* last = GetLastUndo())
	{
		Undos.Emplace(last->MoveNumber + 1, last->FiftyMoveCounter + 1, IsNextMoveBlack() ? last->FenMoveNumber : last->FenMoveNumber + 1);
	}
	else
	{
		Undos.Emplace(0, DEFAULT_FEN_HALF_MOVE, DEFAULT_FEN_MOVE_NUMBER);
	}
	return Undos.Last();
}

FCGUndo* ACGChessBoard::GetLastUndo()
{
	if (Undos.Num() == 0)
	{
		return nullptr;
	}
	return &Undos.Last();
}

void ACGChessBoard::UndoInternal(const FCGUndo& oUndo)
{
	FCGUndo dummyUndo;
	if (oUndo.Promotion)
	{
		Pieces.Remove(oUndo.Promotion);
		oUndo.Promotion->Destroy();
		oUndo.Piece->UnCapture();
	}
	if (oUndo.Piece)
	{
		oUndo.Piece->MoveToTileInternal(oUndo.From, dummyUndo, false);
		oUndo.Piece->Flags = oUndo.Flags;
	}
	if (oUndo.Capture)
	{
		oUndo.Capture->UnCapture();
		oUndo.Capture->MoveToTileInternal(oUndo.To, dummyUndo, false);
	}
	if (oUndo.CastleRook)
	{
		oUndo.CastleRook->MoveToTileInternal(oUndo.CastleRookTile, dummyUndo, false);
	}
}

void ACGChessBoard::UndoTo(const int iMoveNum)
{
	for (int i = Undos.Num() - 1; i >= 0; --i)
	{
		const FCGUndo& u = Undos[i];
		if (u.MoveNumber < iMoveNum)
		{
			break;
		}
		else if (!u.Imported) 
		{
			UndoInternal(u);
			if (u.Piece)
			{
				u.Piece->ClientSnapToPlace();
			}
			if (u.Capture)
			{
				u.Capture->ClientSnapToPlace();
			}
			if (u.CastleRook)
			{
				u.CastleRook->ClientSnapToPlace();
			}
			Undos.RemoveAt(i);
		}
	}
	//listen server has to update the ui
	if (UCGBPUtils::IsLocalUpdateRequired(this))
	{
		UndoNotify();
	}
}

void ACGChessBoard::RequestUndoTo(int iMoveNum)
{

}

void ACGChessBoard::UndoNotify()
{
	if (UWorld* w = GetWorld())
	{
		if (ACGChessPlayerController* pc = w->GetFirstPlayerController<ACGChessPlayerController>())
		{
			pc->OnMove.Broadcast();
		}
	}
}

void ACGChessBoard::PiecesNotify()
{
	//repaint them just in case TODO:optimize?
	RefreshPieceColors();
}

void ACGChessBoard::RebuildAttackMap(bool iIsBlack)
{
	for (ACGTile* t : Board)
	{
		if (t)
		{
			t->AttackedBy.Empty();
		}
	}
	for (ACGPiece* p : Pieces)
	{
		if (p && p->IsBlack() == iIsBlack)
		{
			p->FillAttackMap();
		}
	}
}

bool ACGChessBoard::IsNextMoveBlack() const
{
	if (Undos.Num() == 0)
	{
		return false;
	}
	else
	{
		return !Undos.Last().LastMoveIsBlack;
	}
}

bool ACGChessBoard::IsReadyForNextMove() const
{
	if (ACGGameState* state = GetWorld()->GetGameState<ACGGameState>())
	{
		if (state->IsMatchInProgress())
		{
			if (Undos.Num() == 0 || Undos.Last().Imported)
			{
				return true;
			}
			else
			{
				return Undos.Last().Promotion != Undos.Last().Piece;
			}
		}
	}
	return false;
}

void ACGChessBoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACGChessBoard, Pieces)
	DOREPLIFETIME(ACGChessBoard, CapturedWhite)
	DOREPLIFETIME(ACGChessBoard, CapturedBlack)
	DOREPLIFETIME(ACGChessBoard, Board)
	DOREPLIFETIME(ACGChessBoard, Undos)
	DOREPLIFETIME(ACGChessBoard, WhiteKing)
	DOREPLIFETIME(ACGChessBoard, BlackKing)
}
