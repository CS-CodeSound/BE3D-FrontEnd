#include "ReadWriteJson.h"
#include "ReadWriteFile.h"
#include "Serialization/JsonSerializer.h" // Json
#include "Engine/DataTable.h" // Include DataTable support
#include "AssetRegistry/AssetRegistryModule.h"
#include "PackageHelperFunctions.h" 

#include "JsonObjectConverter.h" // JsonUtilities

UReadWriteJson* UReadWriteJson::Instance = nullptr;

UReadWriteJson* UReadWriteJson::GetInstance()
{
    if (Instance == nullptr)
    {
        Instance = NewObject<UReadWriteJson>();
        Instance->AddToRoot(); // Prevent garbage collection
    }
    return Instance;
}

FBE3DTestStruct UReadWriteJson::ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
    UReadWriteJson* instance = GetInstance();

    // Try to read generic json object from file
    TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, bOutSuccess, OutInfoMessage);
    if (!bOutSuccess)
    {
        return FBE3DTestStruct();
    }

    FBE3DTestStruct RetBE3DTestStruct;

    // Create DataTables for each struct
    UDataTable* CategoryDataTable = NewObject<UDataTable>();
    UDataTable* TickerDataTable = NewObject<UDataTable>();
    UDataTable* Pt13fDataTable = NewObject<UDataTable>();
    UDataTable* CompanyInfoDataTable = NewObject<UDataTable>();

    CategoryDataTable->RowStruct = FCategoryDataList::StaticStruct();
    TickerDataTable->RowStruct = FTickerData::StaticStruct();
    Pt13fDataTable->RowStruct = FPt13fData::StaticStruct();
    CompanyInfoDataTable->RowStruct = FCompanyInfo::StaticStruct();

    FDataTablesWrapper DataTablesWrapper;

    // Process categories and add to DataTable
    const TSharedPtr<FJsonObject>* Categories;
    if (JsonObject->TryGetObjectField(TEXT("categories"), Categories))
    {
        for (const auto& Category : Categories->Get()->Values)
        {
            const FString CategoryName = Category.Key;
            const TArray<TSharedPtr<FJsonValue>> CategoryArray = Category.Value->AsArray();

            FCategoryDataList CategoryDataList;

            // Process each ticker in the category
            for (const auto& Item : CategoryArray)
            {
                const TSharedPtr<FJsonObject> TickerObject = Item->AsObject();
                if (TickerObject.IsValid())
                {
                    FCategoryData CategoryData;
                    CategoryData.Ticker = TickerObject->GetStringField(TEXT("ticker"));
                    CategoryData.Year = TickerObject->GetNumberField(TEXT("year"));
                    CategoryData.Quarter = TickerObject->GetNumberField(TEXT("quarter"));

                    CategoryDataList.CategoryDataArray.Add(CategoryData);
                }
            }

            // Add to RetBE3DTestStruct
            RetBE3DTestStruct.Categories.Add(CategoryName, CategoryDataList);

            // Add CategoryDataList to CategoryDataTable
            CategoryDataTable->AddRow(FName(*CategoryName), CategoryDataList);
        }
    }

    // Process tickers and add to DataTable
    const TSharedPtr<FJsonObject>* Tickers;
    if (JsonObject->TryGetObjectField(TEXT("tickers"), Tickers))
    {
        for (const auto& Ticker : Tickers->Get()->Values)
        {
            const FString TickerName = Ticker.Key;
            const TSharedPtr<FJsonObject> TickerObject = Ticker.Value->AsObject();

            if (TickerObject.IsValid())
            {
                FTickerData TickerData;

                UDataTable* EarningsDataTable = NewObject<UDataTable>();
                EarningsDataTable->RowStruct = FEarningsData::StaticStruct();

                UDataTable* PricesDataTable = NewObject<UDataTable>();
                PricesDataTable->RowStruct = FPriceData::StaticStruct();

                // Process earnings
                const TArray<TSharedPtr<FJsonValue>> EarningsArray = TickerObject->GetArrayField(TEXT("earnings"));
                for (const auto& EarningsItem : EarningsArray)
                {
                    const TSharedPtr<FJsonObject> EarningsObject = EarningsItem->AsObject();
                    if (EarningsObject.IsValid())
                    {
                        FEarningsData EarningsData;
                        EarningsData.Date = EarningsObject->GetStringField(TEXT("date"));
                        EarningsData.EPS = EarningsObject->GetStringField(TEXT("eps"));
                        EarningsData.Revenue = EarningsObject->GetStringField(TEXT("revenue"));

                        TickerData.Earnings.Add(EarningsData);

                        if (!EarningsDataTable->GetRowMap().Contains(FName(*EarningsData.Date)))
                        {
                            TickerData.Earnings.Add(EarningsData);
                            EarningsDataTable->AddRow(FName(*EarningsData.Date), EarningsData);
                        }

                        UE_LOG(LogTemp, Log, TEXT("Added Earnings Data for Ticker:%s, Date: %s, EPS: %s, Revenue: %s"),
                            *TickerName , *EarningsData.Date, *EarningsData.EPS, *EarningsData.Revenue);
                    }
                }

                // Process ratings
                const TArray<TSharedPtr<FJsonValue>> RatingsArray = TickerObject->GetArrayField(TEXT("ratings"));
                for (const auto& RatingItem : RatingsArray)
                {
                    const TSharedPtr<FJsonObject> RatingObject = RatingItem->AsObject();
                    if (RatingObject.IsValid())
                    {
                        FRatingData RatingData;
                        RatingData.Date = RatingObject->GetStringField(TEXT("date"));
                        RatingData.Analyst = RatingObject->GetStringField(TEXT("analyst"));
                        RatingData.RatingCurrent = RatingObject->GetStringField(TEXT("rating_current"));
                        RatingData.RatingPrior = RatingObject->GetStringField(TEXT("rating_prior"));
                        RatingData.AdjustedPTCurrent = RatingObject->GetStringField(TEXT("adjusted_pt_current"));
                        RatingData.AdjustedPTPrior = RatingObject->GetStringField(TEXT("adjusted_pt_prior"));
                        RatingData.ActionCompany = RatingObject->GetStringField(TEXT("action_company"));
                        RatingData.ActionPT = RatingObject->GetStringField(TEXT("action_pt"));

                        TickerData.Ratings.Add(RatingData);
                    }
                }

                // Process prices
                const TArray<TSharedPtr<FJsonValue>> PricesArray = TickerObject->GetArrayField(TEXT("prices"));
                for (const auto& PriceItem : PricesArray)
                {
                    const TSharedPtr<FJsonObject> PriceObject = PriceItem->AsObject();
                    if (PriceObject.IsValid())
                    {
                        FPriceData PriceData;
                        PriceData.Date = PriceObject->GetStringField(TEXT("date"));

                        // Ensure AdjustedClose field exists and is valid (double)
                        if (PriceObject->HasField(TEXT("adjusted_close")))
                        {
                            PriceData.AdjustedClose = PriceObject->GetNumberField(TEXT("adjusted_close"));
                        }

                        // Ensure DividendAmount field exists and is valid (float)
                        if (PriceObject->HasField(TEXT("dividend_amount")))
                        {
                            PriceData.DividendAmount = PriceObject->GetNumberField(TEXT("dividend_amount"));
                        }

                        TickerData.Prices.Add(PriceData);

                        PricesDataTable->AddRow(FName(*PriceData.Date), PriceData);
                    }
                }

                RetBE3DTestStruct.Tickers.Add(TickerName, TickerData);

                DataTablesWrapper.EarningsDataTables.Add(TickerName, EarningsDataTable);
                DataTablesWrapper.PricesDataTables.Add(TickerName, PricesDataTable);

                if (EarningsDataTable)
                {
                    if (EarningsDataTable->GetName().IsEmpty())
                    {
                        FString DefaultDataTableName = FString::Printf(TEXT("EarningsDataTable_%s"), *TickerName);
                        EarningsDataTable->Rename(*DefaultDataTableName);
                    }

                    FString DataTableName = EarningsDataTable->GetName();
                    int32 RowCount = EarningsDataTable->GetRowMap().Num();
                    UE_LOG(LogTemp, Warning, TEXT("Added Earnings DataTable for Ticker: %s, DataTable Name: %s, Row Count: %d"), *TickerName, *DataTableName, RowCount);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to add Earnings DataTable for Ticker: %s, DataTable is null"), *TickerName);
                }
            }

            RetBE3DTestStruct.DataTables = DataTablesWrapper;
        }
    }

    // Process Pt13f data
    const TSharedPtr<FJsonObject>* Pt13fDataObject;
    if (JsonObject->TryGetObjectField(TEXT("pt_13f"), Pt13fDataObject))
    {
        for (const auto& IdPair : Pt13fDataObject->Get()->Values)
        {
            const FString IdKey = IdPair.Key;
            const TArray<TSharedPtr<FJsonValue>>& DataArray = IdPair.Value->AsArray();

            // Create a new FPt13fDataWrapper for this IdKey
            FPt13fDataWrapper DataWrapper;

            for (const auto& Item : DataArray)
            {
                const TSharedPtr<FJsonObject> Pt13fObject = Item->AsObject();
                if (Pt13fObject.IsValid())
                {
                    FPt13fData Pt13fData;
                    Pt13fData.ID = Pt13fObject->GetNumberField(TEXT("id"));
                    Pt13fData.PubDate = Pt13fObject->GetStringField(TEXT("pubDate"));
                    Pt13fData.LatestUpdate = Pt13fObject->GetStringField(TEXT("latestUpdate"));
                    Pt13fData.Period = Pt13fObject->GetStringField(TEXT("period"));
                    Pt13fData.Ticker = Pt13fObject->GetStringField(TEXT("ticker"));
                    Pt13fData.Name = Pt13fObject->GetStringField(TEXT("name"));
                    Pt13fData.Value = Pt13fObject->GetNumberField(TEXT("value"));
                    Pt13fData.Amount = Pt13fObject->GetNumberField(TEXT("amount"));
                    Pt13fData.PricePerShare = Pt13fObject->GetNumberField(TEXT("price_per_share"));
                    Pt13fData.Percent = Pt13fObject->GetNumberField(TEXT("percent"));
                    Pt13fData.NewOrOld = Pt13fObject->GetStringField(TEXT("neworold"));
                    Pt13fData.Diff = Pt13fObject->GetNumberField(TEXT("diff"));

                    // Add the Pt13fData to the DataArray of DataWrapper
                    DataWrapper.DataArray.Add(Pt13fData);
                }
            }

            // Ensure PT13FData is of type FPt13fDataList
            FPt13fDataList NewPt13fDataList;
            NewPt13fDataList.Pt13fDataMap.Add(IdKey, DataWrapper);

            // Add NewPt13fDataList to PT13FData
            RetBE3DTestStruct.PT13FData.Add(IdKey, NewPt13fDataList);
        }
    }

    // Process company info and add to DataTable
    const TArray<TSharedPtr<FJsonValue>>* CompanyInfoArray;
    if (JsonObject->TryGetArrayField(TEXT("us_company_info"), CompanyInfoArray))
    {
        for (const auto& Item : *CompanyInfoArray)
        {
            const TSharedPtr<FJsonObject> CompanyInfoObject = Item->AsObject();
            if (CompanyInfoObject.IsValid())
            {
                FCompanyInfo NewCompanyInfo;
                NewCompanyInfo.Ticker = CompanyInfoObject->GetStringField(TEXT("ticker"));
                NewCompanyInfo.Company = CompanyInfoObject->GetStringField(TEXT("company"));
                NewCompanyInfo.Exchange = CompanyInfoObject->GetStringField(TEXT("exchange"));
                NewCompanyInfo.Sector = CompanyInfoObject->GetStringField(TEXT("sector"));
                NewCompanyInfo.Industry = CompanyInfoObject->GetStringField(TEXT("industry"));
                NewCompanyInfo.LastUpdate = CompanyInfoObject->GetStringField(TEXT("last_update"));
                NewCompanyInfo.Skip = CompanyInfoObject->GetNumberField(TEXT("skip"));
                NewCompanyInfo.CompanyKor = CompanyInfoObject->GetStringField(TEXT("companyKor"));
                NewCompanyInfo.Nickname = CompanyInfoObject->GetStringField(TEXT("nickname"));
                NewCompanyInfo.Intro = CompanyInfoObject->GetStringField(TEXT("intro"));

                FString IntroText = CompanyInfoObject->GetStringField(TEXT("intro"));
                UE_LOG(LogTemp, Log, TEXT("Intro Text: %s"), *IntroText);
                NewCompanyInfo.Intro = IntroText;

                if (!RetBE3DTestStruct.CompanyInfo.Contains(NewCompanyInfo.Ticker))
                {
                    RetBE3DTestStruct.CompanyInfo.Add(NewCompanyInfo.Ticker, NewCompanyInfo);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Duplicate Ticker found: %s. Updating existing entry."), *NewCompanyInfo.Ticker);
                    RetBE3DTestStruct.CompanyInfo[NewCompanyInfo.Ticker] = NewCompanyInfo;
                }
                // Add CompanyInfo to CompanyInfoDataTable
                CompanyInfoDataTable->AddRow(FName(*NewCompanyInfo.Ticker), NewCompanyInfo);
            }
        }
    }

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Read Struct Json Succeeded - '%s'"), *JsonFilePath);
    return RetBE3DTestStruct;
}


// Function to save DataTable as an asset
void UReadWriteJson::SaveDataTableToAsset(UDataTable* DataTable, FString Path)
{
    if (!DataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("DataTable is null. Cannot save."));
        return;
    }

    // Create an Asset Reference
    FStringAssetReference AssetRef(Path);
    UObject* Asset = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), nullptr, *AssetRef.ToString()));
    UPackage* Package = nullptr;

    // Check if the asset already exists
    if (Asset)
    {
        Package = Asset->GetOutermost();
    }
    else
    {
        // Create a new package if it does not exist
        Package = CreatePackage(*Path);
        if (!Package || Package->IsUnreachable())
        {
            UE_LOG(LogTemp, Error, TEXT("Package is invalid or unreachable: %s"), *Path);
            return;
        }

    }

    // Mark the DataTable as created and dirty
    FAssetRegistryModule::AssetCreated(DataTable);
    DataTable->MarkPackageDirty();

    // Check if the package name is valid
    if (FPackageName::IsValidLongPackageName(Path))
    {
        // Convert path to filename
        FString Filename = FPackageName::LongPackageNameToFilename(Path, TEXT(".uasset"));

        // Save the package
        const bool bSuccess = UPackage::SavePackage(Package, DataTable, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *Filename);

        if (!bSuccess)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save package for path: %s"), *Path);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid package name: %s"), *Path);
    }
}


void UReadWriteJson::WriteStructToJsonFile(FString JsonFilePath, FBE3DTestStruct Struct, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Try to convert struct to generic json object
    TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
    if (JsonObject == nullptr)
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("Write Struct Json Failed - Was not able to convert the struct to a json object. This shouldn't really happen."));
        return;
    }

    // Try to write json to file
    WriteJson(JsonFilePath, JsonObject, bOutSuccess, OutInfoMessage);
}

TSharedPtr<FJsonObject> UReadWriteJson::ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Try to read file
    FString JsonString = UReadWriteFile::ReadStringFromFile(JsonFilePath, bOutSuccess, OutInfoMessage);
    if (!bOutSuccess)
    {
        return nullptr;
    }

    TSharedPtr<FJsonObject> RetJsonObject;

    // Try to convert string to json object
    if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), RetJsonObject))
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("Read Json Failed - Was not able to deserialize the json string. Is it the right format? - '%s'"), *JsonString);
        return nullptr;
    }

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Read Json Succeeded - '%s'"), *JsonFilePath);
    return RetJsonObject;
}

void UReadWriteJson::WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
    FString JsonString;

    // Try to convert json object to string
    if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
    {
        bOutSuccess = false;
        OutInfoMessage = FString::Printf(TEXT("Write Json Failed - Was not able to serialize the json to string. Is the JsonObject valid?"));
        return;
    }

    // Try to write json string to file
    UReadWriteFile::WriteStringToFile(JsonFilePath, JsonString, bOutSuccess, OutInfoMessage);
    if (!bOutSuccess)
    {
        return;
    }

    bOutSuccess = true;
    OutInfoMessage = FString::Printf(TEXT("Write Json Succeeded - '%s'"), *JsonFilePath);
}
