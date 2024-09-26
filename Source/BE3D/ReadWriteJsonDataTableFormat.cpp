// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadWriteJsonDataTableFormat.h"
#include "ImportExportDataTable.h"
#include "ReadWriteJson.h"
#include "ReadWriteFile.h"

#include "Engine/DataTable.h" // Engine

TMap<FString, FBE3DTestStruct> UReadWriteJsonDataTableFormat::ReadStructFromJsonFile_DataTableFormat(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Read file
    FString JsonString = UReadWriteFile::ReadStringFromFile(FilePath, bOutSuccess, OutInfoMessage);
    if (!bOutSuccess)
    {
        return TMap<FString, FBE3DTestStruct>();
    }

    // Create data table and tell it which struct it's using
    UDataTable* Table = NewObject<UDataTable>();
    Table->RowStruct = FBE3DTestStruct::StaticStruct();

    // Populate data table
    Table->CreateTableFromJSONString(JsonString);

    // Retrieve the rows
    TArray<FName> RowNames = Table->GetRowNames();

    // Populate the return map
    TMap<FString, FBE3DTestStruct> RowsToStruct;

    for (FName RowName : RowNames)
    {
        FBE3DTestStruct* Row = Table->FindRow<FBE3DTestStruct>(RowName, nullptr);

        if (Row != nullptr)
        {
            RowsToStruct.Add(RowName.ToString(), *Row);
        }
    }

    // Return the rows
    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Read Data Table Json Succeeded - '%s'"), *FilePath);
    return RowsToStruct;
}



void UReadWriteJsonDataTableFormat::WriteStructToJsonFile_DataTableFormat(FString FilePath, TMap<FString, FBE3DTestStruct> RowsToStruct, bool& bOutSuccess, FString& OutInfoMessage)
{
	// Convert all rows to string
	TArray<FString> RowNames;
	RowsToStruct.GetKeys(RowNames);

	// Create data table and tell it which struct it's using
	UDataTable* Table = NewObject<UDataTable>();
	Table->RowStruct = FBE3DTestStruct::StaticStruct();

	// Add all rows to the data table
	for (FString RowName : RowNames)
	{
		Table->AddRow(FName(RowName), RowsToStruct[RowName]);
	}

	// Export to file
	UImportExportDataTable::ExportDataTableToJsonOrCsv(FilePath, Table, bOutSuccess, OutInfoMessage);
}
