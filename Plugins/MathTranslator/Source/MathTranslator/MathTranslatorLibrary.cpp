// Copyright 2026 LostInThought. All Rights Reserved.

#include "MathTranslatorLibrary.h"
#include "Math/UnrealMathUtility.h"

int32 UMathTranslatorLibrary::FindMatchingClosingBrace(const FString& Str, int32 OpenBraceIndex)
{
	int32 Depth = 0;
	for (int32 i = OpenBraceIndex; i < Str.Len(); ++i)
	{
		if (Str[i] == TEXT('{') || Str[i] == TEXT('('))
		{
			Depth++;
		}
		else if (Str[i] == TEXT('}') || Str[i] == TEXT(')'))
		{
			Depth--;
			if (Depth == 0)
			{
				return i;
			}
		}
	}
	return INDEX_NONE;
}

FString UMathTranslatorLibrary::TranslateMathStringToUE(FString InputMath)
{
	FString Parsed = InputMath;

	// ---------------------------------------------------------
	// 1. Clean Layout Modifiers, Formatting & Operators
	// ---------------------------------------------------------
	Parsed = Parsed.Replace(TEXT("\\left"), TEXT(""), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\right"), TEXT(""), ESearchCase::IgnoreCase)
	               .Replace(TEXT(" "), TEXT(""))
	               .Replace(TEXT("\\cdot"), TEXT("*"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\times"), TEXT("*"), ESearchCase::IgnoreCase);

	// Constants across LibreOffice (%pi, %e), LaTeX (\pi, e), Matlab (pi), and Excel
	Parsed = Parsed.Replace(TEXT("%pi"), TEXT("3.14159265f"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("%e"), TEXT("2.71828182f"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\pi"), TEXT("3.14159265f"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("PI()"), TEXT("3.14159265f"), ESearchCase::IgnoreCase);

	// ---------------------------------------------------------
	// 2. Common Scientific & Greek Variables
	// ---------------------------------------------------------
	Parsed = Parsed.Replace(TEXT("\\theta"), TEXT("Theta"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\phi"), TEXT("Phi"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\alpha"), TEXT("Alpha"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\beta"), TEXT("Beta"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\gamma"), TEXT("Gamma"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\lambda"), TEXT("Lambda"), ESearchCase::IgnoreCase);

	// ---------------------------------------------------------
	// 3. LibreOffice Math / OpenOffice (StarMath) Formatting
	// ---------------------------------------------------------
	// LibreOffice fraction syntax: {Numerator} over {Denominator} -> ((Numerator) / (Denominator))
	while (Parsed.Contains(TEXT("over"), ESearchCase::IgnoreCase))
	{
		int32 OverIndex = Parsed.Find(TEXT("over"), ESearchCase::IgnoreCase);
		int32 FirstClose = Parsed.Find(TEXT("}"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, OverIndex);
		int32 FirstOpen = Parsed.Find(TEXT("{"), ESearchCase::IgnoreCase, ESearchDir::FromEnd, FirstClose);
		int32 SecondOpen = Parsed.Find(TEXT("{"), ESearchCase::IgnoreCase, ESearchDir::FromStart, OverIndex);
		int32 SecondClose = (SecondOpen != INDEX_NONE) ? FindMatchingClosingBrace(Parsed, SecondOpen) : INDEX_NONE;

		if (FirstOpen != INDEX_NONE && FirstClose != INDEX_NONE && SecondOpen != INDEX_NONE && SecondClose != INDEX_NONE)
		{
			FString Numerator = Parsed.Mid(FirstOpen + 1, FirstClose - FirstOpen - 1);
			FString Denominator = Parsed.Mid(SecondOpen + 1, SecondClose - SecondOpen - 1);

			FString Replacement = FString::Printf(TEXT("((%s) / (%s))"), *Numerator, *Denominator);
			Parsed.RemoveAt(FirstOpen, (SecondClose - FirstOpen) + 1);
			Parsed.InsertAt(FirstOpen, Replacement);
		}
		else
		{
			break;
		}
	}

	// ---------------------------------------------------------
	// 4. Nested LaTeX Fraction Parsing (\frac{...}{...})
	// ---------------------------------------------------------
	// LaTeX fraction syntax: \frac{Numerator}{Denominator} -> ((Numerator) / (Denominator))
	while (Parsed.Contains(TEXT("\\frac"), ESearchCase::IgnoreCase))
	{
		int32 FracIndex = Parsed.Find(TEXT("\\frac"), ESearchCase::IgnoreCase);
		int32 FirstOpen = Parsed.Find(TEXT("{"), ESearchCase::IgnoreCase, ESearchDir::FromStart, FracIndex);
		int32 FirstClose = (FirstOpen != INDEX_NONE) ? FindMatchingClosingBrace(Parsed, FirstOpen) : INDEX_NONE;

		int32 SecondOpen = (FirstClose != INDEX_NONE) ? Parsed.Find(TEXT("{"), ESearchCase::IgnoreCase, ESearchDir::FromStart, FirstClose) : INDEX_NONE;
		int32 SecondClose = (SecondOpen != INDEX_NONE) ? FindMatchingClosingBrace(Parsed, SecondOpen) : INDEX_NONE;

		if (FirstOpen != INDEX_NONE && FirstClose != INDEX_NONE && SecondOpen != INDEX_NONE && SecondClose != INDEX_NONE)
		{
			FString Numerator = Parsed.Mid(FirstOpen + 1, FirstClose - FirstOpen - 1);
			FString Denominator = Parsed.Mid(SecondOpen + 1, SecondClose - SecondOpen - 1);

			FString Replacement = FString::Printf(TEXT("((%s) / (%s))"), *Numerator, *Denominator);
			Parsed.RemoveAt(FracIndex, (SecondClose - FracIndex) + 1);
			Parsed.InsertAt(FracIndex, Replacement);
		}
		else
		{
			break;
		}
	}

	// ---------------------------------------------------------
	// 5. Exponent & Power Conversion (x^{y} or x^y -> Pow(x, y))
	// ---------------------------------------------------------
	while (Parsed.Contains(TEXT("^{")))
	{
		int32 CaretIndex = Parsed.Find(TEXT("^{"));
		int32 CloseBrace = FindMatchingClosingBrace(Parsed, CaretIndex + 1);
		if (CloseBrace != INDEX_NONE)
		{
			FString Exponent = Parsed.Mid(CaretIndex + 2, CloseBrace - CaretIndex - 2);

			int32 BaseStart = CaretIndex - 1;
			if (BaseStart >= 0 && (Parsed[BaseStart] == TEXT(')') || Parsed[BaseStart] == TEXT('}')))
			{
				int32 Depth = 0;
				for (int32 i = BaseStart; i >= 0; --i)
				{
					if (Parsed[i] == TEXT(')') || Parsed[i] == TEXT('}')) Depth++;
					else if (Parsed[i] == TEXT('(') || Parsed[i] == TEXT('{'))
					{
						Depth--;
						if (Depth == 0) { BaseStart = i; break; }
					}
				}
			}
			else
			{
				while (BaseStart > 0 && (FChar::IsAlnum(Parsed[BaseStart - 1]) || Parsed[BaseStart - 1] == TEXT('_') || Parsed[BaseStart - 1] == TEXT('.')))
				{
					BaseStart--;
				}
			}

			FString Base = Parsed.Mid(BaseStart, CaretIndex - BaseStart);
			FString Replacement = FString::Printf(TEXT("FMath::Pow(%s, (%s))"), *Base, *Exponent);
			Parsed.RemoveAt(BaseStart, (CloseBrace - BaseStart) + 1);
			Parsed.InsertAt(BaseStart, Replacement);
		}
		else
		{
			break;
		}
	}

	// ---------------------------------------------------------
	// 6. Matlab / Octave / Excel Case-Insensitive Functions
	// ---------------------------------------------------------
	Parsed = Parsed.Replace(TEXT("POWER("), TEXT("pow("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("SQRT("), TEXT("sqrt("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("SIN("), TEXT("sin("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("COS("), TEXT("cos("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("TAN("), TEXT("tan("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("ASIN("), TEXT("asin("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("ACOS("), TEXT("acos("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("ATAN("), TEXT("atan("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("ABS("), TEXT("abs("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("LOG10("), TEXT("log10("), ESearchCase::IgnoreCase)
	               .Replace(TEXT("LN("), TEXT("log("), ESearchCase::IgnoreCase);

	// ---------------------------------------------------------
	// 7. LaTeX Function Prefix Cleanup (\sin -> sin)
	// ---------------------------------------------------------
	Parsed = Parsed.Replace(TEXT("\\sin"), TEXT("sin"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\cos"), TEXT("cos"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\tan"), TEXT("tan"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\asin"), TEXT("asin"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\acos"), TEXT("acos"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\atan"), TEXT("atan"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\sqrt"), TEXT("sqrt"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\abs"), TEXT("abs"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\log"), TEXT("log"), ESearchCase::IgnoreCase)
	               .Replace(TEXT("\\ln"), TEXT("log"), ESearchCase::IgnoreCase);

	// ---------------------------------------------------------
	// 8. Final Native C++ FMath Mapping
	// ---------------------------------------------------------
	Parsed = Parsed.Replace(TEXT("sin("), TEXT("FMath::Sin("))
	               .Replace(TEXT("cos("), TEXT("FMath::Cos("))
	               .Replace(TEXT("tan("), TEXT("FMath::Tan("))
	               .Replace(TEXT("asin("), TEXT("FMath::Asin("))
	               .Replace(TEXT("acos("), TEXT("FMath::Acos("))
	               .Replace(TEXT("atan("), TEXT("FMath::Atan("))
	               .Replace(TEXT("sqrt("), TEXT("FMath::Sqrt("))
	               .Replace(TEXT("abs("), TEXT("FMath::Abs("))
	               .Replace(TEXT("pow("), TEXT("FMath::Pow("))
	               .Replace(TEXT("log("), TEXT("FMath::Loge("))
	               .Replace(TEXT("log10("), TEXT("FMath::Logx(10.0f, "))
	               .Replace(TEXT("exp("), TEXT("FMath::Exp("))
	               .Replace(TEXT("floor("), TEXT("FMath::FloorToFloat("))
	               .Replace(TEXT("ceil("), TEXT("FMath::CeilToFloat("))
	               .Replace(TEXT("round("), TEXT("FMath::RoundToFloat("));

	return Parsed;
}

FVector UMathTranslatorLibrary::ConvertLatLongToLocalVector(float Latitude, float Longitude, float GlobeRadius, float Altitude)
{
	// Convert Geographic Coordinates (Lat/Lon) to Unreal Cartesian 3D Space (Z-Up, X-Forward)
	const float TotalRadius = GlobeRadius + Altitude;
	const float LatRad = FMath::DegreesToRadians(Latitude);
	const float LonRad = FMath::DegreesToRadians(Longitude);

	const float X = TotalRadius * FMath::Cos(LatRad) * FMath::Cos(LonRad);
	const float Y = TotalRadius * FMath::Cos(LatRad) * FMath::Sin(LonRad);
	const float Z = TotalRadius * FMath::Sin(LatRad);

	return FVector(X, Y, Z);
}