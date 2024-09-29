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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Date;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString EPS;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Revenue;
};

USTRUCT(BlueprintType)
struct FRatingData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Date;

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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Date;

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
struct FPt13fData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 ID = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString PubDate;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString LatestUpdate;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Period;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Ticker;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    double Value = 0.0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    double Amount = 0.0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float PricePerShare = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Percent = 0.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString NewOrOld;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Diff = 0.0f;
};

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
    TArray<FCategoryData> CategoryDataArray; // FCategoryData 배열을 포함하는 구조체
};

USTRUCT(BlueprintType)
struct FPt13fDataWrapper : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FPt13fData> DataArray; // Array of FPt13fData
};


USTRUCT(BlueprintType)
struct FPt13fDataList : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FPt13fDataWrapper> Pt13fDataMap; // "id_0", "id_1" 등
};

// 최종 구조체 정의
USTRUCT(BlueprintType)
struct FBE3DTestStruct : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FCategoryDataList> Categories; // 카테고리 이름과 그 데이터들을 저장하는 구조체 매핑

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FTickerData> Tickers; // 티커별 데이터 저장

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FPt13fDataList> PT13FData; // 테이블 이름과 그 데이터를 저장하는 구조체 매핑

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FPtListData> PTList; // PT 리스트 데이터 저장

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FCompanyInfo> CompanyInfo; // 티커를 key로 하는 회사 정보 저장
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

    static void SaveDataTableToAsset(UDataTable* DataTable, FString Path);

};
