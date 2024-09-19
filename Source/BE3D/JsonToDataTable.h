#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "JsonToDataTable.generated.h"

UCLASS()
class BE3D_API UJsonToDataTable : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // Function to be called at the start of the game to load JSON into DataTable
    UFUNCTION(BlueprintCallable, Category = "BE3D - Json To DataTable")
    static UDataTable* LoadJsonToDataTable(FString JsonFilePath, bool& bOutSuccess, FString& OutInfoMessage);
};
