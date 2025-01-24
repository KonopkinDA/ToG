#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToG_SourcePushActor.generated.h"

UCLASS()
class TOG_API AToG_SourcePushActor : public AActor
{

	GENERATED_BODY()
	
public:
	AToG_SourcePushActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
