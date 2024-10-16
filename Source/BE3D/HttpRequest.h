// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "ReadWriteJson.h"

#include "UObject/NoExportTypes.h"
#include "HttpRequest.generated.h"

/**
 * 
 */
UCLASS()
class BE3D_API UHttpRequest : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "BE3D - Http Request")
	static UHttpRequest* GetInstance()
	{
		if (!SingletonInstance)
		{
			SingletonInstance = NewObject<UHttpRequest>();
			SingletonInstance->AddToRoot();
		}
		return SingletonInstance;
	}


	UFUNCTION(BlueprintCallable, Category = "BE3D - Http Request")
	void SendGetRequest(const FString& StartDate, const FString& EndDate, int32 Guru);

private:
	static UHttpRequest* SingletonInstance;

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
