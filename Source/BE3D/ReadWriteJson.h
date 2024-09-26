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
struct FCategoryData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Ticker;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Year;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Quarter;
};

USTRUCT(BlueprintType)
struct FEarningsData
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
struct FRatingData
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
struct FPriceData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Date;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    double AdjustedClose;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float DividendAmount;
};

USTRUCT(BlueprintType)
struct FTickerData
{
    GENERATED_BODY()

    // �� �б� �����͸� ���
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FEarningsData> Earnings;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FRatingData> Ratings;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FPriceData> Prices;
};

USTRUCT(BlueprintType)
struct FPt13fData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 ID;

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
    double Value;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    double Amount;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float PricePerShare;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Percent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString NewOrOld;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Diff;
};

USTRUCT(BlueprintType)
struct FPtListData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 ID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Benchmark;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Title;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString CIK;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString LatestUpdate;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Balance;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Cash;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Person;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float YRAvg;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float MDD;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float Dur;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float StDev;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float MonthProfit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float YearProfit;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 YRAvgRank;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 MonthRank;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 YearRank;

    // �߰� �ʵ�
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString NewOrOld; // JSON���� "neworold"�� �ش�

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float SharpeRatio;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 SharpeRatioRank;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    float YRSharpeRatio;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 YRSharpeRatioRank;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    int32 Cnt13f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Latest13f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString RebalComment;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString FullName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Philosophy;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Comment;
};


USTRUCT(BlueprintType)
struct FCompanyInfo
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
    int32 Skip;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString CompanyKor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Nickname;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    FString Intro;
};

USTRUCT(BlueprintType)
struct FCategoryDataList
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FCategoryData> CategoryDataArray; // FCategoryData �迭�� �����ϴ� ����ü
};

USTRUCT(BlueprintType)
struct FPt13fDataWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FPt13fData> DataArray; // Array of FPt13fData
};


USTRUCT(BlueprintType)
struct FPt13fDataList
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FPt13fDataWrapper> Pt13fDataMap; // "id_0", "id_1" ��
};

// ���� ����ü ����
USTRUCT(BlueprintType)
struct FBE3DTestStruct : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FCategoryDataList> Categories; // ī�װ� �̸��� �� �����͵��� �����ϴ� ����ü ����

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FTickerData> Tickers; // ƼĿ�� ������ ����

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TMap<FString, FPt13fDataList> PT13FData; // ���̺� �̸��� �� �����͸� �����ϴ� ����ü ����

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FPtListData> PTList; // PT ����Ʈ ������ ����

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
    TArray<FCompanyInfo> CompanyInfo; // ȸ�� ���� ����
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

};
