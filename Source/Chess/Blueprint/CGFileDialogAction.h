
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CGFileDialogAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompleted, const FString&, Path, bool, bSuccess);

UCLASS()
class CHESS_API UCGFileDialogAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	FString Path;

public:
	void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "Chess utils")
	static UCGFileDialogAction* OpenFileChooser(UObject* WorldContextObject);

	UPROPERTY(BlueprintAssignable)
	FOnCompleted Completed;

	
};
