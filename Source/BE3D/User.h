// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Portfolio.h"
#include "User.generated.h"

/**
 *
 */


UCLASS()
class BE3D_API UUser : public UObject
{
    GENERATED_BODY()

private:
    float Cash;
    UPortfolio* UserPortfolio;
    int Year;
    int Month;
    int Day;

    static UUser* Instance;

public:
    UUser();

    UFUNCTION(BlueprintCallable, Category = "User")
    static UUser* GetInstance() {
        if (!Instance)
        {
            Instance = NewObject<UUser>();
            Instance->AddToRoot(); // GC에 의해 수집되지 않도록 고정
        }
        return Instance;
    }

    // Initialize the user with initial cash and date
    UFUNCTION(BlueprintCallable, Category = "User")
    void InitializeUser(double InitialCash, int StartYear, int StartMonth, int StartDay);

    // Create Portfolio from stock data
    UFUNCTION(BlueprintCallable, Category = "User")
    void CreateUserPortfolio(const TMap<FString, float>& StockPriceMap, const TMap<FString, float>& StockPercentMap);

    // Sell all stocks in Portfolio
    UFUNCTION(BlueprintCallable, Category = "User")
    double SellAllStocksPortfolio(int NowYear, int NowMonth, int NowDay);

    // Calculate all dividends in Portfolio
    UFUNCTION(BlueprintCallable, Category = "User")
    double AllDividendsPortfolio(int NowYear, int NowMonth, int NowDay);

    // Get cash amount
    UFUNCTION(BlueprintCallable, Category = "User")
    double GetCash() const { return Cash; }

    // Update cash amount
    UFUNCTION(BlueprintCallable, Category = "User")
    void CashUpdate(double NewCash) { Cash = NewCash; }

    // Getters for the date
    UFUNCTION(BlueprintCallable, Category = "User")
    int GetYear() const { return Year; }

    UFUNCTION(BlueprintCallable, Category = "User")
    int GetMonth() const { return Month; }

    UFUNCTION(BlueprintCallable, Category = "User")
    int GetDay() const { return Day; }

    // Update date
    UFUNCTION(BlueprintCallable, Category = "User")
    void UpdateDate(int NewYear, int NewMonth, int NewDay);


    UFUNCTION(BlueprintCallable, Category = "Stock Management")
    void UpdateStockPercentages(const TArray<FString>& cardstobuy, TMap<FString, float>& StockPercentages);
};
