// Copyright 2026 LostInThought. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MathTranslatorLibrary.generated.h"

/**
 * Multi-Language Math Expression Translator API
 * Converts LaTeX, LibreOffice, Excel, Matlab, ASCIIMath, and Plain-Text math into Unreal Engine FMath syntax.
 */
UCLASS()
class MATHTRANSLATOR_API UMathTranslatorLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Translates any input expression (LaTeX, LibreOffice, Excel, Matlab, Plain Text) into Unreal C++ / FMath format.
	 * @param InputMath Raw math string from any supported language/format.
	 * @return Clean FString reformatted into standard Unreal FMath:: syntax.
	 */
	UFUNCTION(BlueprintPure, Category = "Math Translation")
	static FString TranslateMathStringToUE(FString InputMath);

	/**
	 * Converts Geographic Coordinates (Latitude, Longitude, Altitude) to 3D Cartesian Vectors (X, Y, Z).
	 * Uses Unreal Engine's standard Z-Up, X-Forward coordinate system.
	 */
	UFUNCTION(BlueprintPure, Category = "Math Translation|Globe", meta = (DisplayName = "Convert Lat Long To Local Vector"))
	static FVector ConvertLatLongToLocalVector(float Latitude, float Longitude, float GlobeRadius = 100.0f, float Altitude = 5.0f);

private:
	/** Helper to find the matching closing curly brace/parenthesis accounting for nesting depth */
	static int32 FindMatchingClosingBrace(const FString& Str, int32 OpenBraceIndex);
};