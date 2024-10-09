// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequest.h"

void UHttpRequest::SendGetRequest(const FString& Url)
{
    // Http ��� ��������
    FHttpModule* Http = &FHttpModule::Get();

    // Http ��û ����
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::OnResponseReceived);

    // URL ����
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

        // �޸𸮿� ����� JsonObject�� �ٷ� �ѱ��

        // ReadStructFromJsonFile ȣ��
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
        // �Ľ� ���� ��
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON from Response."));
    }
}