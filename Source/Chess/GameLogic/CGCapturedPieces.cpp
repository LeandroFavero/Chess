
#include "CGCapturedPieces.h"
#include "ChessLogic/CGPiece.h"
#include "Net/UnrealNetwork.h"

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

void UCGCapturedPieces::Add(ACGPiece* p)
{
	Pieces.Add(p);
	//update positions if listen server, otherwise it updates from the notify
	if (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
	{
		RefreshPositions();
	}
}

void UCGCapturedPieces::Remove(ACGPiece* p)
{
	Pieces.Remove(p);
	//update positions if listen server, otherwise it updates from the notify
	if (GEngine->GetNetMode(GetWorld()) == NM_ListenServer)
	{
		RefreshPositions();
	}
}

void UCGCapturedPieces::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCGCapturedPieces, Pieces)
}