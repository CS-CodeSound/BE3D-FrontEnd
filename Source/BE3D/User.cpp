// Fill out your copyright notice in the Description page of Project Settings.


#include "User.h"
#include "Portfolio.h"

UUser* UUser::Instance = nullptr;

UUser::UUser()
    : Cash(0.0), UserPortfolio(nullptr), Year(2019), Month(3), Day(31)
{
}

void UUser::InitializeUser(double InitialCash, int StartYear, int StartMonth, int StartDay)
{
    Cash = InitialCash;
    Year = StartYear;
    Month = StartMonth;
    Day = StartDay;
}

void UUser::CreateUserPortfolio(const TMap<FString, float>& StockPriceMap, const TMap<FString, float>& StockPercentMap)
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
        UserPortfolio->InitializePortfolio(StockPercentMap, StockPercentMap, Cash, Year, Month, Day);
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

void UUser::UpdateStockPercentages(const TArray<FString>& cardstobuy, TMap<FString, float>& StockPercentages)
{
    TMap<FString, float> StockPercentagesMap;

    // 주식 티커 저장 시 할당할 퍼센트 값
    float PercentagePerStock = 0.1f;

    // cardstobuy 배열을 순회하며 주식 티커와 퍼센트를 계산
    for (const FString& Ticker : cardstobuy)
    {
        // 만약 티커가 이미 Map에 존재한다면, 기존 퍼센트에 0.1을 더한다
        if (StockPercentagesMap.Contains(Ticker))
        {
            StockPercentagesMap[Ticker] += PercentagePerStock;
        }
        // 티커가 Map에 없다면 새로 추가하고 퍼센트를 0.1로 설정
        else
        {
            StockPercentagesMap.Add(Ticker, PercentagePerStock);
        }
    }

    StockPercentages.Empty();
    StockPercentages = StockPercentagesMap;

    // 결과 출력 (디버깅용)
    for (const TPair<FString, float>& Stock : StockPercentages)
    {
        UE_LOG(LogTemp, Log, TEXT("Ticker: %s, Percent: %.1f%%"), *Stock.Key, Stock.Value * 100.0f);
    }
}