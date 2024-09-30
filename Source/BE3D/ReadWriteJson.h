// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"

#include "ReadWriteJson.generated.h"

class FJsonObject;

/**
 *
 */
USTRUCT(BlueprintType)
struct FCategoryData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Ticker;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Year = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Quarter = 0;
};

USTRUCT(BlueprintType)
struct FEarningsData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Year;

    UPROPERTY(BlueprintReadWrite)
    int32 Month;

    UPROPERTY(BlueprintReadWrite)
    int32 Day;

    UPROPERTY(BlueprintReadWrite)
    double AdjustedClose;  // Stock price by the quarter

    UPROPERTY(BlueprintReadWrite)
    float DividendAmount;  // Dividend by the quarter

    UPROPERTY(BlueprintReadWrite)
    bool GuruHolding;  // Guru holding status

    UPROPERTY(BlueprintReadWrite)
    FString EPS;  // Earnings per share

    UPROPERTY(BlueprintReadWrite)
    FString Revenue;  // Revenue
};

USTRUCT(BlueprintType)
struct FRatingData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Year;

    UPROPERTY(BlueprintReadWrite)
    int32 Month;

    UPROPERTY(BlueprintReadWrite)
    int32 Day;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Analyst;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString RatingCurrent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString RatingPrior;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString AdjustedPTCurrent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString AdjustedPTPrior;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString ActionCompany;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString ActionPT;
};

USTRUCT(BlueprintType)
struct FPriceData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 Year;

    UPROPERTY(BlueprintReadWrite)
    int32 Month;

    UPROPERTY(BlueprintReadWrite)
    int32 Day;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    double AdjustedClose = 0.0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float DividendAmount = 0.0f;
};

USTRUCT(BlueprintType)
struct FTickerData : public FTableRowBase
{
    GENERATED_BODY()

    // 전 분기 데이터만 사용
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FEarningsData> Earnings;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FRatingData> Ratings;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FPriceData> Prices;
};

USTRUCT(BlueprintType)
struct FGuruPortfolioData
{
    GENERATED_BODY()

    // To be filled later when the data is defined
};

/*
USTRUCT(BlueprintType)
struct FPtListData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Benchmark = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Balance = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cash = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YRAvg = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MDD = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Dur = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StDev = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MonthProfit = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YearProfit = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 YRAvgRank = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MonthRank = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 YearRank = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SharpeRatio = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SharpeRatioRank = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YRSharpeRatio = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 YRSharpeRatioRank = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Cnt13f = 0;
};
*/

USTRUCT(BlueprintType)
struct FCompanyInfo : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Ticker;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Company;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Exchange;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Sector;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Industry;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString LastUpdate;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Skip = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString CompanyKor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Nickname;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Intro;
};

USTRUCT(BlueprintType)
struct FCategoryDataList : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FCategoryData> CategoryDataArray; // Array of FCategoryData
};

USTRUCT(BlueprintType)
struct FDataTablesWrapper : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, UDataTable*> EarningsDataTables;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, UDataTable*> PricesDataTables;
};

// 최종 구조체 정의
USTRUCT(BlueprintType)
struct FBE3DTestStruct : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
	TMap<FString, FCategoryDataList> Categories;  // Category names mapping to a list of tickers, years, and quarters

	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FTickerData> Tickers;  // Ticker names mapping to earnings, ratings, and price data

	UPROPERTY(BlueprintReadWrite)
	FGuruPortfolioData GuruPortfolio;  // Placeholder for Guru Portfolio information

	UPROPERTY(BlueprintReadWrite)
    TMap<FString, FCompanyInfo> CompanyInfo;  // List of company information

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FDataTablesWrapper DataTables;
};



UCLASS()
class BE3D_API UReadWriteJson : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    /**
     * Open a json file, read its content and dump it into a structure
     *
     * @param JsonFilePath		The path of your json file: "C:/Temp/MyJson.json"
     * @param bOutSuccess		If the action was a success or not
     * @param OutInfoMessage	More information about the action's result
     *
     * @return The structure content of your json file
     */
    UFUNCTION(BlueprintCallable, Category = "BE3D - Read Write Json")
    static FBE3DTestStruct ReadStructFromJsonFile(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);

    /**
     * Convert your structure to a string, create a json file and dump the string in it
     *
     * @param JsonFilePath		The path of your json file: "C:/Temp/MyJson.json"
     * @param Struct			The struct to want to write in the file
     * @param bOutSuccess		If the action was a success or not
     * @param OutInfoMessage	More information about the action's result
     */
    UFUNCTION(BlueprintCallable, Category = "BE3D - Read Write Json")
    static void WriteStructToJsonFile(FString JsonFilePath, FBE3DTestStruct Struct, bool& bOutSuccess, FString& OutInfoMessage);


public:

    /**
     * Open a json file, read its content and convert it to a JsonObject
     *
     * @param JsonFilePath		The path of your json file: "C:/Temp/MyJson.json"
     * @param bOutSuccess		If the action was a success or not
     * @param OutInfoMessage	More information about the action's result
     *
     * @return The JsonObject content of your json file
     */
    static TSharedPtr<FJsonObject> ReadJson(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);

    /**
     * Convert your JsonObject to a string, create a json file and dump the string in it
     *
     * @param JsonFilePath		The path of your json file: "C:/Temp/MyJson.json"
     * @param JsonObject		The JsonObject to want to write in the file
     * @param bOutSuccess		If the action was a success or not
     * @param OutInfoMessage	More information about the action's result
     */
    static void WriteJson(FString JsonFilePath, TSharedPtr<FJsonObject> JsonObject, bool& bOutSuccess, FString& OutInfoMessage);


    UPROPERTY(BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
    TMap<FString, UDataTable*> EarningsDataTables;

    UPROPERTY(BlueprintReadOnly, Category = "DataTable", meta = (AllowPrivateAccess = "true"))
    TMap<FString, UDataTable*> PricesDataTables;

    UFUNCTION(BlueprintCallable, Category = "DataTable")
    void SetEarningsDataTable(const FString& TickerName, UDataTable* NewDataTable)
    {
        if (NewDataTable != nullptr)
        {
            EarningsDataTables.Add(TickerName, NewDataTable);
            UE_LOG(LogTemp, Log, TEXT("Set Earnings DataTable for Ticker: %s"), *TickerName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to set Earnings DataTable for Ticker: %s, DataTable is null"), *TickerName);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "DataTable")
    void SetPricesDataTable(const FString& TickerName, UDataTable* NewDataTable)
    {
        if (NewDataTable != nullptr)
        {
            PricesDataTables.Add(TickerName, NewDataTable);
            UE_LOG(LogTemp, Log, TEXT("Set Prices DataTable for Ticker: %s"), *TickerName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to set Prices DataTable for Ticker: %s, DataTable is null"), *TickerName);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "BE3D - Read Write Json")
    static UReadWriteJson* GetInstance();

private:
    static UReadWriteJson* Instance; // Singleton instance

};
