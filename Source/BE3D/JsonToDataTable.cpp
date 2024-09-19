#include "JsonToDataTable.h"
#include "Engine/DataTable.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "ReadWriteJson.h"
#include "JsonObjectConverter.h"

UDataTable* UJsonToDataTable::LoadJsonToDataTable(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Read JSON from file
    FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *JsonFilePath))
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("Failed to load JSON file: %s"), *JsonFilePath);
        return nullptr;
    }

    // Parse the JSON string
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
    {
        bOutSuccess = false;
        OutInfoMessage = TEXT("Failed to parse JSON");
        return nullptr;
    }

    // Create a new DataTable
    UDataTable* DataTable = NewObject<UDataTable>();
    DataTable->RowStruct = FBE3DTestStruct::StaticStruct();  // Set the struct you want to map the JSON to

    // Extract rows from JSON and populate DataTable
    TArray<TSharedPtr<FJsonValue>> Rows = JsonObject->GetArrayField("Rows");
    for (const TSharedPtr<FJsonValue>& RowValue : Rows)
    {
        TSharedPtr<FJsonObject> RowObject = RowValue->AsObject();
        FBE3DTestStruct RowStruct;

        // Convert the JSON object to the struct
        if (FJsonObjectConverter::JsonObjectToUStruct(RowObject.ToSharedRef(), FBE3DTestStruct::StaticStruct(), &RowStruct, 0, 0))
        {
            // Add the row to the DataTable
            FName RowName = FName(*RowObject->GetStringField("RowName"));
            DataTable->AddRow(RowName, RowStruct);
        }
        else
        {
            bOutSuccess = false;
            OutInfoMessage = FString::Printf(TEXT("Failed to convert JSON to struct"));
            return nullptr;
        }
    }

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Successfully loaded JSON file into DataTable: %s"), *JsonFilePath);
    return DataTable;
}
