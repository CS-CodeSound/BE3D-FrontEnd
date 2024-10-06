// Fill out your copyright notice in the Description page of Project Settings.


#include "User.h"
#include "Portfolio.h"

UUser::UUser()
    : Cash(0.0), UserPortfolio(nullptr), Year(2024), Month(9), Day(13)
{
}

void UUser::InitializeUser(double InitialCash, int StartYear, int StartMonth, int StartDay)
{
    Cash = InitialCash;
    Year = StartYear;
    Month = StartMonth;
    Day = StartDay;
}

void UUser::CreateUserPortfolio(const TArray<FStockData>& StockDataArray)
{
    // Clean up existing UserPortfolio if it exists
    if (UserPortfolio)
    {
        UserPortfolio->ConditionalBeginDestroy();
        UserPortfolio = nullptr;
    }

    // Create a new instance of UPortfolio
    UserPortfolio = NewObject<UPortfolio>(this);
    if (UserPortfolio)
    {
        // Initialize the portfolio with the stock data
        UserPortfolio->InitializePortfolio(StockDataArray, Cash, Year, Month, Day);
    }
}

double UUser::SellAllStocksPortfolio(int NowYear, int NowMonth, int NowDay)
{
    return UserPortfolio ? UserPortfolio->SellAllStocks(NowYear, NowMonth, NowDay) : 0.0;
}

double UUser::AllDividendsPortfolio(int NowYear, int NowMonth, int NowDay)
{
    return UserPortfolio ? UserPortfolio->AllDividends(NowYear, NowMonth, NowDay) : 0.0;
}

void UUser::UpdateDate(int NewYear, int NewMonth, int NewDay)
{
    Year = NewYear;
    Month = NewMonth;
    Day = NewDay;
}