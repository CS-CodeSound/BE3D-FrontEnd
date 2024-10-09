// Fill out your copyright notice in the Description page of Project Settings.


#include "PurchasedStock.h"

UPurchasedStock::UPurchasedStock()
{
    // Initialize default values if needed
    Price = 0.0f;
    Dividend = 0.0f;
    StockQuantity = 0.0f;
}

void UPurchasedStock::InitializeStock(const FString& InTicker, float Money, float Percent, int InYear, int InMonth, int InDay)
{
    Ticker = InTicker;
    Year = InYear;
    Month = InMonth;
    Day = InDay;

    // �ӽ� �ֽ� ���� (DB ������ ���� ��� �ӽ� �� ���)
    Price = 23.3f;
    StockQuantity = Money * Percent / Price;
}

float UPurchasedStock::CalculateDividend(int SellYear, int SellMonth, int SellDay)
{
    return Dividend * StockQuantity;
}

float UPurchasedStock::SellStock(int SellYear, int SellMonth, int SellDay)
{
    float TempPrice = 24.2f;
    return StockQuantity * TempPrice;
}