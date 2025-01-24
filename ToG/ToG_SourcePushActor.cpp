#include "ToG_SourcePushActor.h"
//#include "G:\Unreal\Projects\ToG_54\Intermediate\Build\Win64\x64\ToGEditor\Development\UnrealEd\SharedPCH.UnrealEd.Project.NoValFmtStr.ValApi.Cpp20.InclOrderUnreal5_3.h"
#include "TechnicalFunctions.h"


// Sets default values
AToG_SourcePushActor::AToG_SourcePushActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AToG_SourcePushActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AToG_SourcePushActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
