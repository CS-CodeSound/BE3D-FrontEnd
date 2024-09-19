// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteJsonDataTableFormat.generated.h"

struct FBE3DTestStruct;

/**
 * 
 */
UCLASS()
class BE3D_API UReadWriteJsonDataTableFormat : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Read a series of struct based on a json file formatted like a data table
	 *
	 * @param FilePath			The path of the source file: "C:/Temp/MyJson.json"
	 * @param bOutSuccess		If the action was a success or not
	 * @param OutInfoMessage	More information about the action's result
	 *
	 * @return The structs
	 */
	UFUNCTION(BlueprintCallable, Category = "BE3D - Read Write Data Table Json")
	static TMap<FString, FBE3DTestStruct> ReadStructFromJsonFile_DataTableFormat(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);


	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Write a series of struct to a json file. Will be formatted like a data table
	 *
	 * @param FilePath			The path of the output file: "C:/Temp/MyJson.json"
	 * @param RowsToStruct		The structs to write in the json
	 * @param bOutSuccess		If the action was a success or not
	 * @param OutInfoMessage	More information about the action's result
	 */
	UFUNCTION(BlueprintCallable, Category = "BE3D - Read Write Data Table Json")
	static void WriteStructToJsonFile_DataTableFormat(FString FilePath, TMap<FString, FBE3DTestStruct> RowsToStruct, bool& bOutSuccess, FString& OutInfoMessage);

};
