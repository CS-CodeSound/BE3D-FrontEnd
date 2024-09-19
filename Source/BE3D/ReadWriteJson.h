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
struct FPriceData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString Date;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	float AdjustedClose;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	float DividendAmount;
};

USTRUCT(BlueprintType)
struct FRatingData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString Date;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString Exchange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString Analyst;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString RatingCurrent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString RatingPrior;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString ActionCompany;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString ActionPT;
};

USTRUCT(BlueprintType)
struct FEarningsData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FString EPS;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	int64 Revenue; // 큰 숫자를 위해 int64 사용
};

USTRUCT(BlueprintType)
struct FYearData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	FEarningsData Earnings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	TArray<FRatingData> Ratings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	TMap<FString, FPriceData> Price; // 날짜별로 가격 데이터를 저장
};

USTRUCT(BlueprintType)
struct FTickerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	TMap<FString, FYearData> YearData; // 연도별로 데이터를 저장
};

USTRUCT(BlueprintType)
struct FBE3DTestStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BE3D")
	TMap<FString, FTickerData> Tickers; // 티커별로 데이터를 저장
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
