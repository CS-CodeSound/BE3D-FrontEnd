// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PurchasedStock.h"
#include "Portfolio.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStockData
{
    GENERATED_BODY()

    TMap<FString, float> StockMap; // Wrap TMap in a struct
};

UCLASS()
class BE3D_API UPortfolio : public UObject
{
	GENERATED_BODY()
	
private:
    TArray<UPurchasedStock*> Holdings;
    int Year;
    int Month;
    int Day;

public:
    UPortfolio();

    UFUNCTION(BlueprintCallable)
    void InitializePortfolio(const TArray<FStockData>& StockDataArray, double TotalMoney, int StartYear, int StartMonth, int StartDay);

    UFUNCTION(BlueprintCallable)
    float SellAllStocks(int NowYear, int NowMonth, int NowDay);

    UFUNCTION(BlueprintCallable)
    float AllDividends(int NowYear, int NowMonth, int NowDay);
};
