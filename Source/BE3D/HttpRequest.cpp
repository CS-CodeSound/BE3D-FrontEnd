// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequest.h"

void UHttpRequest::SendGetRequest(const FString& Url)
{
    // Http 모듈 가져오기
    FHttpModule* Http = &FHttpModule::Get();

    // Http 요청 생성
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);

    // URL 설정
    Request->SetURL(Url);

    // GET 방식 설정
    Request->SetVerb(TEXT("GET"));

    // 요청 전송
    Request->ProcessRequest();
}

void UHttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP Request failed."));
        return;
    }

    // 서버 응답에서 받은 콘텐츠
    FString ResponseContent = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("Response Content: %s"), *ResponseContent);

    // JSON 데이터를 파싱하기 위한 reader 생성
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

    // JSON 파싱 시도
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // 파싱 성공 시
        UE_LOG(LogTemp, Log, TEXT("JSON Parse Successful"));

        // 성공 여부와 메시지 변수 준비
        bool bOutSuccess;
        FString OutInfoMessage;

        // 메모리에 저장된 JsonObject를 바로 넘기기

        // ReadStructFromJsonFile 호출
        UReadWriteJson* ReadWriteJson = UReadWriteJson::GetInstance(); // Singleton
        FBE3DTestStruct ParsedData = ReadWriteJson->ReadStructFromJsonFile(TEXT(""), bOutSuccess, OutInfoMessage);

        if (bOutSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("Data Struct Successfully Created from JSON: %s"), *OutInfoMessage);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON into Struct: %s"), *OutInfoMessage);
        }
    }
    else
    {
        // 파싱 실패 시
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from Response."));
    }
}