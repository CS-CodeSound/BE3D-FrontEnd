// Fill out your copyright notice in the Description page of Project Settings.

#include "PurchasedStock.h"

UPurchasedStock::UPurchasedStock()
{
    // Initialize default values if needed
    Price = 0.0f;
    Dividend = 0.0f;
    StockQuantity = 0.0f;
}

void UPurchasedStock::InitializeStock(const FString& InTicker, float InPrice, float Money, float Percent, int InYear, int InMonth, int InDay)
{
    Ticker = InTicker;
    Year = InYear;
    Month = InMonth;
    Day = InDay;
    Price = InPrice;
    StockQuantity = Money * Percent / Price;
}

float UPurchasedStock::CalculateDividend(int SellYear, int SellMonth, int SellDay)
{
    return Dividend * StockQuantity;
}

float UPurchasedStock::SellStock(int SellYear, int SellMonth, int SellDay)
{
    if (UReadWriteJson::PricesData.Contains(Ticker)) // Assuming TickerName is the key
    {
        // Get the price array for the specific ticker
        const TArray<FPriceData>& PriceArray = UReadWriteJson::PricesData[Ticker];

        FPriceData SelectedPriceData; // To hold the price data for the selected date
        bool bFound = false; // Flag to track if a price was found

        // Iterate through the price data to find the most recent price before the sell date
        for (const FPriceData& PriceData : PriceArray)
        {
            // Check if the current PriceData is before the sell date
            if ((PriceData.Year < SellYear) ||
                (PriceData.Year == SellYear && PriceData.Month < SellMonth) ||
                (PriceData.Year == SellYear && PriceData.Month == SellMonth && PriceData.Day < SellDay))
            {
                SelectedPriceData = PriceData; // Update the selected price data
                bFound = true; // Mark as found
            }
        }

        // Check if a valid price was found before the sell date
        if (bFound)
        {
            float Price_ = SelectedPriceData.AdjustedClose; // Use AdjustedClose for sale value
            return StockQuantity * Price_; // Calculate and return the total sale value
        }

    }

    // Log a warning if no price data is found before the sell date
   //  UE_LOG(LogTemp, Warning, TEXT("No price data found for Ticker: %s before %04d-%02d-%02d"), *TickerName, SellYear, SellMonth, SellDay);
    return 0.0f; // Handle appropriately if no price is found
}