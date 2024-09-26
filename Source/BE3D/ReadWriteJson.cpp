#include "ReadWriteJson.h"
#include "ReadWriteFile.h"
#include "Serialization/JsonSerializer.h" // Json
#include "JsonObjectConverter.h" // JsonUtilities

#include "ReadWriteJson.h"
#include "ReadWriteFile.h"
#include "Serialization/JsonSerializer.h" // Json
#include "JsonObjectConverter.h" // JsonUtilities

FBE3DTestStruct UReadWriteJson::ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage)
{
    // Try to read generic json object from file
    TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilePath, bOutSuccess, OutInfoMessage);
    if (!bOutSuccess)
    {
        return FBE3DTestStruct();
    }

    FBE3DTestStruct RetBE3DTestStruct;

    // Read categories
    const TSharedPtr<FJsonObject>* Categories;
    if (JsonObject->TryGetObjectField(TEXT("categories"), Categories))
    {
        for (const auto& Category : Categories->Get()->Values)
        {
            const FString CategoryName = Category.Key;
            const TArray<TSharedPtr<FJsonValue>> CategoryArray = Category.Value->AsArray();

            FCategoryDataList CategoryDataList; // FCategoryDataList 구조체 사용

            // Process each ticker in the category
            for (const auto& Item : CategoryArray)
            {
                const TSharedPtr<FJsonObject> TickerObject = Item->AsObject();
                if (TickerObject.IsValid())
                {
                    FString TickerName = TickerObject->GetStringField(TEXT("ticker"));
                    int32 Year = TickerObject->GetNumberField(TEXT("year"));
                    int32 Quarter = TickerObject->GetNumberField(TEXT("quarter"));

                    FCategoryData CategoryData;
                    CategoryData.Ticker = TickerName;
                    CategoryData.Year = Year;
                    CategoryData.Quarter = Quarter;

                    // CategoryData를 CategoryDataList에 추가
                    CategoryDataList.CategoryDataArray.Add(CategoryData);
                }
            }

            // Add the data to RetBE3DTestStruct
            RetBE3DTestStruct.Categories.Add(CategoryName, CategoryDataList);
        }
    }

    // Read tickers
    const TSharedPtr<FJsonObject>* Tickers;
    if (JsonObject->TryGetObjectField(TEXT("tickers"), Tickers))
    {
        for (const auto& Ticker : Tickers->Get()->Values)
        {
            const FString TickerName = Ticker.Key;
            const TSharedPtr<FJsonObject> TickerObject = Ticker.Value->AsObject();

            if (TickerObject.IsValid())
            {
                FTickerData& TickerData = RetBE3DTestStruct.Tickers.FindOrAdd(TickerName);

                // Read earnings
                const TArray<TSharedPtr<FJsonValue>> EarningsArray = TickerObject->GetArrayField(TEXT("earnings"));
                for (const auto& EarningsItem : EarningsArray)
                {
                    const TSharedPtr<FJsonObject> EarningsObject = EarningsItem->AsObject();
                    if (EarningsObject.IsValid())
                    {
                        FEarningsData EarningsData;
                        EarningsData.Date = EarningsObject->GetStringField(TEXT("date"));
                        EarningsData.EPS = EarningsObject->GetStringField(TEXT("eps"));
                        EarningsData.Revenue = EarningsObject->GetStringField(TEXT("revenue")); // revenue는 string으로 정의

                        TickerData.Earnings.Add(EarningsData); // 직접 Earnings 배열에 추가
                    }
                }

                // Read ratings
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

                // Read prices
                const TArray<TSharedPtr<FJsonValue>> PricesArray = TickerObject->GetArrayField(TEXT("prices"));
                for (const auto& PriceItem : PricesArray)
                {
                    const TSharedPtr<FJsonObject> PriceObject = PriceItem->AsObject();
                    if (PriceObject.IsValid())
                    {
                        FPriceData PriceData;
                        PriceData.Date = PriceObject->GetStringField(TEXT("date"));
                        PriceData.AdjustedClose = PriceObject->GetNumberField(TEXT("adjusted_close"));
                        PriceData.DividendAmount = PriceObject->GetNumberField(TEXT("dividend_amount"));

                        TickerData.Prices.Add(PriceData);
                    }
                }
            }
        }
    }


    const TSharedPtr<FJsonObject>* Pt13fDataObject;
    if (JsonObject->TryGetObjectField(TEXT("pt_13f"), Pt13fDataObject))
    {
        for (const auto& IdPair : Pt13fDataObject->Get()->Values)
        {
            const FString IdKey = IdPair.Key; // "id_0", "id_1" 등
            const TArray<TSharedPtr<FJsonValue>>& DataArray = IdPair.Value->AsArray();

            // Create a new wrapper for this ID
            FPt13fDataList DataList;
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

                    // Add to the wrapper's array
                    DataWrapper.DataArray.Add(Pt13fData);
                }
                // Add the wrapper to the map
                DataList.Pt13fDataMap.Add(IdKey, DataWrapper);
            }
        }
    }



    // Read FPtListData
    const TArray<TSharedPtr<FJsonValue>>* PtListDataArray;
    if (JsonObject->TryGetArrayField(TEXT("pt_list_data"), PtListDataArray))
    {
        for (const auto& Item : *PtListDataArray)
        {
            const TSharedPtr<FJsonObject> PtListObject = Item->AsObject();
            if (PtListObject.IsValid())
            {
                FPtListData PtListData;
                PtListData.ID = PtListObject->GetNumberField(TEXT("id"));
                PtListData.Benchmark = PtListObject->GetNumberField(TEXT("benchmark"));
                PtListData.Title = PtListObject->GetStringField(TEXT("title"));
                PtListData.CIK = PtListObject->GetStringField(TEXT("cik"));
                PtListData.LatestUpdate = PtListObject->GetStringField(TEXT("latest_update"));
                PtListData.Balance = PtListObject->GetNumberField(TEXT("balance"));
                PtListData.Cash = PtListObject->GetNumberField(TEXT("cash"));
                PtListData.Person = PtListObject->GetStringField(TEXT("person"));
                PtListData.YRAvg = PtListObject->GetNumberField(TEXT("yr_avg"));
                PtListData.MDD = PtListObject->GetNumberField(TEXT("mdd"));
                PtListData.Dur = PtListObject->GetNumberField(TEXT("dur"));
                PtListData.StDev = PtListObject->GetNumberField(TEXT("st_dev"));
                PtListData.MonthProfit = PtListObject->GetNumberField(TEXT("month_profit"));
                PtListData.YearProfit = PtListObject->GetNumberField(TEXT("year_profit"));
                PtListData.YRAvgRank = PtListObject->GetNumberField(TEXT("yr_avg_rank"));
                PtListData.MonthRank = PtListObject->GetNumberField(TEXT("month_rank"));
                PtListData.YearRank = PtListObject->GetNumberField(TEXT("year_rank"));

                RetBE3DTestStruct.PTList.Add(PtListData);
            }
        }
    }

    // Read FCompanyInfo
    const TArray<TSharedPtr<FJsonValue>>* CompanyInfoArray;
    if (JsonObject->TryGetArrayField(TEXT("company_info"), CompanyInfoArray))
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
                NewCompanyInfo.CompanyKor = CompanyInfoObject->GetStringField(TEXT("company_kor"));
                NewCompanyInfo.Nickname = CompanyInfoObject->GetStringField(TEXT("nickname"));
                NewCompanyInfo.Intro = CompanyInfoObject->GetStringField(TEXT("intro"));

                // 배열에 추가
                RetBE3DTestStruct.CompanyInfo.Add(NewCompanyInfo);
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
