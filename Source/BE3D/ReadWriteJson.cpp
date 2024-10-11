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
    UDataTable* CompanyInfoDataTable = NewObject<UDataTable>();

    CategoryDataTable->RowStruct = FCategoryDataList::StaticStruct();
    TickerDataTable->RowStruct = FTickerData::StaticStruct();
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

                UDataTable* IndicateDataTable = NewObject<UDataTable>();
                IndicateDataTable->RowStruct = FIndicateData::StaticStruct();

                // Process earnings
                const TArray<TSharedPtr<FJsonValue>> EarningsArray = TickerObject->GetArrayField(TEXT("earnings"));
                for (const auto& EarningsItem : EarningsArray)
                {
                    const TSharedPtr<FJsonObject> EarningsObject = EarningsItem->AsObject();
                    if (EarningsObject.IsValid())
                    {
                        FEarningsData EarningsData;
                        EarningsData.Year = EarningsObject->GetNumberField(TEXT("year"));
                        EarningsData.Month = EarningsObject->GetNumberField(TEXT("month"));
                        EarningsData.Day = EarningsObject->GetNumberField(TEXT("day"));
                        EarningsData.EPS = EarningsObject->GetStringField(TEXT("eps"));
                        EarningsData.Revenue = EarningsObject->GetStringField(TEXT("revenue"));

                        TickerData.Earnings.Add(EarningsData);

                        // Generate a unique row name using year, month, and day
                        FString DateString = FString::Printf(TEXT("%04d-%02d-%02d"), EarningsData.Year, EarningsData.Month, EarningsData.Day);
                        FName RowName = FName(*DateString);

                        // Check if the row already exists using the generated date as the row name
                        if (!EarningsDataTable->GetRowMap().Contains(RowName))
                        {
                            EarningsDataTable->AddRow(RowName, EarningsData);
                        }

                        UE_LOG(LogTemp, Log, TEXT("Added Earnings Data for Ticker:%s, Date: %s, EPS: %s, Revenue: %s"),
                            *TickerName, *DateString, *EarningsData.EPS, *EarningsData.Revenue);
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
                        RatingData.Year = RatingObject->GetNumberField(TEXT("year"));
                        RatingData.Month = RatingObject->GetNumberField(TEXT("month"));
                        RatingData.Day = RatingObject->GetNumberField(TEXT("day"));
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
                        PriceData.Year = PriceObject->GetNumberField(TEXT("year"));
                        PriceData.Month = PriceObject->GetNumberField(TEXT("month"));
                        PriceData.Day = PriceObject->GetNumberField(TEXT("day"));

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

                        // Generate a unique row name using year, month, and day
                        FString DateString = FString::Printf(TEXT("%04d-%02d-%02d"), PriceData.Year, PriceData.Month, PriceData.Day);
                        FName RowName = FName(*DateString);

                        PricesDataTable->AddRow(RowName, PriceData);

                        UE_LOG(LogTemp, Log, TEXT("Added Prices Data for Ticker: %s, Date: %s, Adjusted Close: %f, Dividend Amount: %f"),
                            *TickerName, *DateString, PriceData.AdjustedClose, PriceData.DividendAmount);
                    }
                }

                // Process indicate data
                const TArray<TSharedPtr<FJsonValue>> IndicateArray = TickerObject->GetArrayField(TEXT("indicate"));
                for (const auto& IndicateItem : IndicateArray)
                {
                    const TSharedPtr<FJsonObject> IndicateObject = IndicateItem->AsObject();
                    if (IndicateObject.IsValid())
                    {
                        FIndicateData IndicateData;
                        IndicateData.Year = IndicateObject->GetNumberField(TEXT("year"));
                        IndicateData.Month = IndicateObject->GetNumberField(TEXT("month"));
                        IndicateData.Day = IndicateObject->GetNumberField(TEXT("day"));
                        IndicateData.AdjustedClose = IndicateObject->GetNumberField(TEXT("adjusted_close"));
                        IndicateData.PER = IndicateObject->GetNumberField(TEXT("per"));
                        IndicateData.YoY = IndicateObject->GetNumberField(TEXT("yoy"));
                        IndicateData.GuruHolding = IndicateObject->GetBoolField(TEXT("guru_holding"));
                        IndicateData.DividendAmount = IndicateObject->GetNumberField(TEXT("dividend_amount"));

                        TickerData.Indicate.Add(IndicateData);

                        FString DateString = FString::Printf(TEXT("%04d-%02d-%02d"), IndicateData.Year, IndicateData.Month, IndicateData.Day);
                        FName RowName = FName(*DateString);

                        IndicateDataTable->AddRow(RowName, IndicateData);

                        UE_LOG(LogTemp, Log, TEXT("Added Indicate Data for Ticker: %s, Date: %s, Adjusted Close: %f, PER: %f, YoY: %f, Dividend Amount: %f"),
                            *TickerName, *DateString, IndicateData.AdjustedClose, IndicateData.PER, IndicateData.YoY, IndicateData.DividendAmount);
                    }
                }

                RetBE3DTestStruct.Tickers.Add(TickerName, TickerData);

                DataTablesWrapper.EarningsDataTables.Add(TickerName, EarningsDataTable);
                DataTablesWrapper.PricesDataTables.Add(TickerName, PricesDataTable);
                DataTablesWrapper.IndicateDataTables.Add(TickerName, IndicateDataTable);

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

                if (PricesDataTable)
                {
                    if (PricesDataTable->GetName().IsEmpty())
                    {
                        FString DefaultDataTableName = FString::Printf(TEXT("PricesDataTable_%s"), *TickerName);
                        PricesDataTable->Rename(*DefaultDataTableName);
                    }

                    FString PricesDataTableName = PricesDataTable->GetName();
                    int32 PricesRowCount = PricesDataTable->GetRowMap().Num();
                    UE_LOG(LogTemp, Warning, TEXT("Added Prices DataTable for Ticker: %s, DataTable Name: %s, Row Count: %d"), *TickerName, *PricesDataTableName, PricesRowCount);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to add Prices DataTable for Ticker: %s, DataTable is null"), *TickerName);
                }
            }

            RetBE3DTestStruct.DataTables = DataTablesWrapper;
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

                FString CompanyText = NewCompanyInfo.Company;

                if (CompanyText.Len() > 20)
                {
                    CompanyText = CompanyText.Left(20) + TEXT("...");
                }
                NewCompanyInfo.Company = CompanyText;

                FString IntroText = CompanyInfoObject->GetStringField(TEXT("intro"));

                if (!IntroText.IsEmpty())
                {
                    FString FormattedIntroText;
                    int32 CurrentLength = 0;

                    for (int32 i = 0; i < IntroText.Len(); ++i)
                    {
                        FormattedIntroText.AppendChar(IntroText[i]);
                        CurrentLength++;

                        if (CurrentLength >= 50)
                        {
                            FormattedIntroText.AppendChar(TEXT('\n'));
                            CurrentLength = 0;  
                        }
                    }

                    UE_LOG(LogTemp, Log, TEXT("Intro Text: %s"), *FormattedIntroText);
                    
                    NewCompanyInfo.Intro = FormattedIntroText;
                }

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

FBE3DTestStruct UReadWriteJson::ParseJsonToStruct(TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
    FBE3DTestStruct RetBE3DTestStruct;

    if (!JsonObject->HasField(TEXT("SomeField")))
    {
        return RetBE3DTestStruct;
    }

    // Create DataTables for each struct
    UDataTable* CategoryDataTable = NewObject<UDataTable>();
    UDataTable* TickerDataTable = NewObject<UDataTable>();
    UDataTable* CompanyInfoDataTable = NewObject<UDataTable>();

    CategoryDataTable->RowStruct = FCategoryDataList::StaticStruct();
    TickerDataTable->RowStruct = FTickerData::StaticStruct();
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

                UDataTable* IndicateDataTable = NewObject<UDataTable>();
                IndicateDataTable->RowStruct = FIndicateData::StaticStruct();

                // Process earnings
                const TArray<TSharedPtr<FJsonValue>> EarningsArray = TickerObject->GetArrayField(TEXT("earnings"));
                for (const auto& EarningsItem : EarningsArray)
                {
                    const TSharedPtr<FJsonObject> EarningsObject = EarningsItem->AsObject();
                    if (EarningsObject.IsValid())
                    {
                        FEarningsData EarningsData;
                        EarningsData.Year = EarningsObject->GetNumberField(TEXT("year"));
                        EarningsData.Month = EarningsObject->GetNumberField(TEXT("month"));
                        EarningsData.Day = EarningsObject->GetNumberField(TEXT("day"));
                        EarningsData.EPS = EarningsObject->GetStringField(TEXT("eps"));
                        EarningsData.Revenue = EarningsObject->GetStringField(TEXT("revenue"));

                        TickerData.Earnings.Add(EarningsData);

                        // Generate a unique row name using year, month, and day
                        FString DateString = FString::Printf(TEXT("%04d-%02d-%02d"), EarningsData.Year, EarningsData.Month, EarningsData.Day);
                        FName RowName = FName(*DateString);

                        // Check if the row already exists using the generated date as the row name
                        if (!EarningsDataTable->GetRowMap().Contains(RowName))
                        {
                            EarningsDataTable->AddRow(RowName, EarningsData);
                        }

                        UE_LOG(LogTemp, Log, TEXT("Added Earnings Data for Ticker:%s, Date: %s, EPS: %s, Revenue: %s"),
                            *TickerName, *DateString, *EarningsData.EPS, *EarningsData.Revenue);
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
                        RatingData.Year = RatingObject->GetNumberField(TEXT("year"));
                        RatingData.Month = RatingObject->GetNumberField(TEXT("month"));
                        RatingData.Day = RatingObject->GetNumberField(TEXT("day"));
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
                        PriceData.Year = PriceObject->GetNumberField(TEXT("year"));
                        PriceData.Month = PriceObject->GetNumberField(TEXT("month"));
                        PriceData.Day = PriceObject->GetNumberField(TEXT("day"));

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

                        // Generate a unique row name using year, month, and day
                        FString DateString = FString::Printf(TEXT("%04d-%02d-%02d"), PriceData.Year, PriceData.Month, PriceData.Day);
                        FName RowName = FName(*DateString);

                        PricesDataTable->AddRow(RowName, PriceData);

                        UE_LOG(LogTemp, Log, TEXT("Added Prices Data for Ticker: %s, Date: %s, Adjusted Close: %f, Dividend Amount: %f"),
                            *TickerName, *DateString, PriceData.AdjustedClose, PriceData.DividendAmount);
                    }
                }

                // Process indicate data
                const TArray<TSharedPtr<FJsonValue>> IndicateArray = TickerObject->GetArrayField(TEXT("indicate"));
                for (const auto& IndicateItem : IndicateArray)
                {
                    const TSharedPtr<FJsonObject> IndicateObject = IndicateItem->AsObject();
                    if (IndicateObject.IsValid())
                    {
                        FIndicateData IndicateData;
                        IndicateData.Year = IndicateObject->GetNumberField(TEXT("year"));
                        IndicateData.Month = IndicateObject->GetNumberField(TEXT("month"));
                        IndicateData.Day = IndicateObject->GetNumberField(TEXT("day"));
                        IndicateData.AdjustedClose = IndicateObject->GetNumberField(TEXT("adjusted_close"));
                        IndicateData.PER = IndicateObject->GetNumberField(TEXT("per"));
                        IndicateData.YoY = IndicateObject->GetNumberField(TEXT("yoy"));
                        IndicateData.GuruHolding = IndicateObject->GetBoolField(TEXT("guru_holding"));
                        IndicateData.DividendAmount = IndicateObject->GetNumberField(TEXT("dividend_amount"));

                        TickerData.Indicate.Add(IndicateData);

                        FString DateString = FString::Printf(TEXT("%04d-%02d-%02d"), IndicateData.Year, IndicateData.Month, IndicateData.Day);
                        FName RowName = FName(*DateString);

                        IndicateDataTable->AddRow(RowName, IndicateData);

                        UE_LOG(LogTemp, Log, TEXT("Added Indicate Data for Ticker: %s, Date: %s, Adjusted Close: %f, PER: %f, YoY: %f, Dividend Amount: %f"),
                            *TickerName, *DateString, IndicateData.AdjustedClose, IndicateData.PER, IndicateData.YoY, IndicateData.DividendAmount);
                    }
                }

                RetBE3DTestStruct.Tickers.Add(TickerName, TickerData);

                DataTablesWrapper.EarningsDataTables.Add(TickerName, EarningsDataTable);
                DataTablesWrapper.PricesDataTables.Add(TickerName, PricesDataTable);
                DataTablesWrapper.IndicateDataTables.Add(TickerName, IndicateDataTable);

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

                if (PricesDataTable)
                {
                    if (PricesDataTable->GetName().IsEmpty())
                    {
                        FString DefaultDataTableName = FString::Printf(TEXT("PricesDataTable_%s"), *TickerName);
                        PricesDataTable->Rename(*DefaultDataTableName);
                    }

                    FString PricesDataTableName = PricesDataTable->GetName();
                    int32 PricesRowCount = PricesDataTable->GetRowMap().Num();
                    UE_LOG(LogTemp, Warning, TEXT("Added Prices DataTable for Ticker: %s, DataTable Name: %s, Row Count: %d"), *TickerName, *PricesDataTableName, PricesRowCount);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to add Prices DataTable for Ticker: %s, DataTable is null"), *TickerName);
                }
            }

            RetBE3DTestStruct.DataTables = DataTablesWrapper;
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

                FString CompanyText = NewCompanyInfo.Company;

                if (CompanyText.Len() > 20)
                {
                    CompanyText = CompanyText.Left(20) + TEXT("...");
                }
                NewCompanyInfo.Company = CompanyText;

                FString IntroText = CompanyInfoObject->GetStringField(TEXT("intro"));

                if (!IntroText.IsEmpty())
                {
                    FString FormattedIntroText;
                    int32 CurrentLength = 0;

                    for (int32 i = 0; i < IntroText.Len(); ++i)
                    {
                        FormattedIntroText.AppendChar(IntroText[i]);
                        CurrentLength++;

                        if (CurrentLength >= 50)
                        {
                            FormattedIntroText.AppendChar(TEXT('\n'));
                            CurrentLength = 0;
                        }
                    }

                    UE_LOG(LogTemp, Log, TEXT("Intro Text: %s"), *FormattedIntroText);

                    NewCompanyInfo.Intro = FormattedIntroText;
                }

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
    OutInfoMessage = FString::Printf(TEXT("Parse Json to Struct Succeeded"));

    return RetBE3DTestStruct;
}