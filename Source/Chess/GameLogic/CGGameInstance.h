// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Chess.h"
#include "CGGameInstance.generated.h"

class UCGSettingsSave;
class FOnlineSessionSearchResult;

USTRUCT(BlueprintType)
struct FJoinableGame
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;

	FOnlineSessionSearchResult SessionInfo;

	FJoinableGame() {};
	FJoinableGame(const FString name, const FOnlineSessionSearchResult sessionInfo) : Name(name), SessionInfo(sessionInfo){};
};
/**
 * 
 */
UCLASS()
class CHESS_API UCGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	FString CurrentFen {};

public:
	UCGGameInstance(const FObjectInitializer& ObjectInitializer);

	void Init() override;

	UPROPERTY(BlueprintReadWrite, Category = "Chess settings")
	UCGSettingsSave* Settings;

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void SaveCfg();

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	bool LoadCfg();

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void JoinIP(const FString& Ip);

	const FName GetMyName() const;

	TSharedPtr<const FUniqueNetId> GetMyId() const;

	TSubclassOf<UOnlineSession> GetOnlineSessionClass() override;

	//https://unreal.gg-labs.com/wiki-archives/networking/how-to-use-sessions-in-c++

	/** Handles to registered delegates for creating/starting a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	/* Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	/* Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	/** Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	/** Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	/** Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	bool Host(const FString& Fen, bool bIsLan);

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void Search();

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess settings")
	void SearchComplete(const TArray<FJoinableGame>& Games, bool bWasSuccessful);

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void Join();

	UFUNCTION(BlueprintCallable, Category = "Chess settings")
	void DestroySession();

	bool HostSession(TSharedPtr<const FUniqueNetId> iUserId, const FName& iSessionName, bool iIsLAN, bool iIsPresence, int32 iMaxNumPlayers);

	virtual void OnCreateSessionComplete(FName iSessionName, bool iWasSuccessful);

	void OnStartOnlineGameComplete(FName iSessionName, bool iWasSuccessful);

	
	void FindSessions(TSharedPtr<const FUniqueNetId> iUserId, bool iIsLAN, bool iIsPresence);

	void OnFindSessionsComplete(bool iWasSuccessful);

	bool JoinSessionInternal(TSharedPtr<const FUniqueNetId> iUserId, FName iSessionName, const FOnlineSessionSearchResult& iSearchResult);

	void OnJoinSessionComplete(FName iSessionName, EOnJoinSessionCompleteResult::Type iResult);

	virtual void OnDestroySessionComplete(FName iSessionName, bool iWasSuccessful);
};
