// Fill out your copyright notice in the Description page of Project Settings.


#include "Portfolio.h"

UPortfolio::UPortfolio()
{
}

void UPortfolio::InitializePortfolio(const TMap<FString, float>& StockPriceMap, const TMap<FString, float>& StockPercentMap, double TotalMoney, int StartYear, int StartMonth, int StartDay)
{
    Year = StartYear;
    Month = StartMonth;
    Day = StartDay;

    // Create UStock objects for each TMap entry
    for (const auto& Elem : StockPriceMap)
    {
        FString Ticker = Elem.Key;
        float Price = Elem.Value;

        const float* PercentPtr = StockPercentMap.Find(Ticker);
        float Percent = (PercentPtr != nullptr) ? *PercentPtr : 0.0f;

        // 林侥 按眉 积己
        UPurchasedStock* Stock = NewObject<UPurchasedStock>(this);
        if (Stock)
        {
            Stock->InitializeStock(Ticker, Price, TotalMoney, Percent, StartYear, StartMonth, StartDay);
            Holdings.Add(Stock);
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