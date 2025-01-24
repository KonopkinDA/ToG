#pragma once

class FToG_SpellDrawing
{
public:
	FString SpellName;
	TArray<FVector2d> DrawingPoints;
	double MaxDifference;

	FToG_SpellDrawing(FString Name, TArray<FVector2d> Points, double Difference = 100);
};
