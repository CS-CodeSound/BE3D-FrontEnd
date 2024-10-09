// Fill out your copyright notice in the Description page of Project Settings.


#include "Portfolio.h"

UPortfolio::UPortfolio()
{
}

void UPortfolio::InitializePortfolio(const TArray<FStockData>& StockDataArray, double TotalMoney, int StartYear, int StartMonth, int StartDay)
{
    Year = StartYear;
    Month = StartMonth;
    Day = StartDay;

    // Create UStock objects for each TMap entry
    for (const auto& StockData : StockDataArray)
    {
        // Access the TMap inside FStockData
        for (const auto& Elem : StockData.StockMap)
        {
            FString Ticker = Elem.Key;
            float Percent = Elem.Value;

            UPurchasedStock* Stock = NewObject<UPurchasedStock>(this);
            if (Stock)
            {
                Stock->InitializeStock(Ticker, TotalMoney, Percent, StartYear, StartMonth, StartDay);
                Holdings.Add(Stock);
            }
        }
    }
}

float UPortfolio::SellAllStocks(int NowYear, int NowMonth, int NowDay)
{
    float TotalValue = 0.0f;
    for (UPurchasedStock* Stock : Holdings)
    {
        TotalValue += Stock->SellStock(NowYear, NowMonth, NowDay);
    }
    return TotalValue;
}

float UPortfolio::AllDividends(int NowYear, int NowMonth, int NowDay)
{
    float TotalValue = 0.0f;
    for (UPurchasedStock* Stock : Holdings)
    {
        TotalValue += Stock->CalculateDividend(NowYear, NowMonth, NowDay);
    }
    return TotalValue;
}