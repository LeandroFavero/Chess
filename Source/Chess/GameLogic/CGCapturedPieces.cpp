
#include "CGCapturedPieces.h"
#include "ChessLogic/CGPiece.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/CGBPUtils.h"

UCGCapturedPieces::UCGCapturedPieces()
{
	SetIsReplicatedByDefault(true);
}

void UCGCapturedPieces::RefreshPositions()
{
	int i = 0;
	for (ACGPiece* p : Pieces)
	{
		if (p)
		{
			p->SetActorRelativeLocation(GetRelativeLocation()+(Offset * i));
			i += 1;
		}
	}
}

void UCGCapturedPieces::CapturedNotify()
{
	RefreshPositions();
}

void UCGCapturedPieces::Add(ACGPiece* iPiece)
{
	Pieces.Add(iPiece);
	//update positions if listen server, otherwise it updates from the notify
	if (UCGBPUtils::IsLocalUpdateRequired(this))
	{
		RefreshPositions();
	}
}

void UCGCapturedPieces::Remove(ACGPiece* iPiece)
{
	Pieces.Remove(iPiece);
	//update positions if listen server, otherwise it updates from the notify
	if (UCGBPUtils::IsLocalUpdateRequired(this))
	{
		RefreshPositions();
	}
}

void UCGCapturedPieces::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCGCapturedPieces, Pieces)
}