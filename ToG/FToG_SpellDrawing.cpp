#include "FToG_SpellDrawing.h"

FToG_SpellDrawing::FToG_SpellDrawing(FString Name, TArray<FVector2d> Points, double Difference)
{
	SpellName = Name;
	DrawingPoints = Points;
	MaxDifference = Difference;
}
