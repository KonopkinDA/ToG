#include "TechnicalFunctions.h"

namespace TechnicalFunctionsNamespace
{
	//Need to be replaced
	float GetAngleBetweenVectorsWithoutZ(const FVector A, const FVector B)
	{
		return  FMath::RadiansToDegrees(FMath::Acos((A.X * B.X + A.Y * B.Y) / (sqrt(A.X * A.X + A.Y * A.Y) * sqrt(B.X * B.X + B.Y * B.Y))));
	}
	float GetAngleBetweenVectorsWithoutY(const FVector A, const FVector B)
	{
		return  FMath::RadiansToDegrees(FMath::Acos((A.X * B.X + A.Z * B.Z) / (sqrt(A.X * A.X + A.Z * A.Z) * sqrt(B.X * B.X + B.Z * B.Z))));
	}
	float GetAngleBetweenVectorsWithoutX(const FVector A, const FVector B)
	{
		return  FMath::RadiansToDegrees(FMath::Acos((A.Y * B.Y + A.Z * B.Z) / (sqrt(A.Y * A.Y + A.Z * A.Z) * sqrt(B.Y * B.Y + B.Z * B.Z))));
	}

	//
	void ConsoleLog(FString Message)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}
	//
	void ConsoleLog(float Message)
	{
		FString SMessage = FString::SanitizeFloat(Message);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SMessage);
	}
	//
	void ConsoleLog(int Message)
	{
		FString SMessage = FString::FromInt(Message);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SMessage);
	}
}

