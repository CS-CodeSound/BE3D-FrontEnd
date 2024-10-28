// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ReadWriteJson.h"
#include "PurchasedStock.generated.h"

/**
 *
 */
UCLASS()
class BE3D_API UPurchasedStock : public UObject
{
    GENERATED_BODY()

private:
    FString Ticker;
    float Price;
    float Dividend;
    float StockQuantity;
    int Year;
    int Month;
    int Day;

public:
    UPurchasedStock();

    UFUNCTION(BlueprintCallable)
    void InitializeStock(const FString& Ticker, float InPrice, float Money, float Percent, int Year, int Month, int Day);

    UFUNCTION(BlueprintCallable)
    float CalculateDividend(int SellYear, int SellMonth, int SellDay);

    UFUNCTION(BlueprintCallable)
    float SellStock(int SellYear, int SellMonth, int SellDay);

    UFUNCTION(BlueprintCallable)
    float GetPrice() const { return Price; }
};
