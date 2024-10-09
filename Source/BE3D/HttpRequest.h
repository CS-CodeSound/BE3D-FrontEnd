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
	void SendGetRequest(const FString& Url);

private:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
