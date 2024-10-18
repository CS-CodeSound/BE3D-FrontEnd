// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequest.h"

UHttpRequest* UHttpRequest::SingletonInstance = nullptr;

void UHttpRequest::SendGetRequest(const FString& StartDate, const FString& EndDate, int32 Guru)
{

    // Http ��� ��������
    FHttpModule* Http = &FHttpModule::Get();
    const FString& BaseUrl = "http://localhost:3000/data";

    // Http ��û ����
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);

    // URL ����
    FString Url = FString::Printf(TEXT("%s?startdate=%s&enddate=%s&guru=%d"), *BaseUrl, *StartDate, *EndDate, Guru);
    Request->SetURL(Url);

    // GET ��� ����
    Request->SetVerb(TEXT("GET"));

    // ��û ����
    Request->ProcessRequest();

}

void UHttpRequest::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP Request failed."));
        return;
    }

    // ���� ���信�� ���� ������
    FString ResponseContent = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("Response Content: %s"), *ResponseContent);

    // JSON �����͸� �Ľ��ϱ� ���� reader ����
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);

    // JSON �Ľ� �õ�
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // �Ľ� ���� ��
        UE_LOG(LogTemp, Log, TEXT("JSON Parse Successful"));

        // ���� ���ο� �޽��� ���� �غ�
        bool bOutSuccess;
        FString OutInfoMessage;

        // ReadStructFromJsonFile ȣ��
        UReadWriteJson* ReadWriteJson = UReadWriteJson::GetInstance(); // Singleton
        if (JsonObject.IsValid())
        {
            BE3DStruct = ReadWriteJson->ParseJsonToStruct(JsonObject, bOutSuccess, OutInfoMessage);
            if (!bOutSuccess)
            {
                UE_LOG(LogTemp, Warning, TEXT("Failed to parse JSON: %s"), *OutInfoMessage);
                return;
            }
            // ���������� �Ľ��� ������ ���� ��� ����
            UE_LOG(LogTemp, Log, TEXT("Parsed JSON Successfully!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to deserialize JSON response."));
        }
    }
    else
    {
        // �Ľ� ���� ��
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from Response."));
    }
}