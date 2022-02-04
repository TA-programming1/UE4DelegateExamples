// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBulletHitEnemySignature, AActor*, EnemyHit);

UCLASS()
class SPACEINVADERS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	FBulletHitEnemySignature OnBulletHitEnemy;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BulletThings)
		UStaticMeshComponent* BulletMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BulletThings)
		class USphereComponent* Collision{};
private: 



	UPROPERTY(EditAnywhere, Category = BulletThings)
		float Speed = 1000.f; 

	UPROPERTY(EditAnywhere, Category = BulletThings)
		float TimeBeforeDestroy = 5.f; 

	UPROPERTY(EditAnywhere, Category = BulletThings)
		float TimeLived = 0.f; 

	UPROPERTY(EditAnywhere, Category = BulletThings)
		UParticleSystem* BulletFireFX = nullptr; 

	UPROPERTY(EditAnywhere, Category = BulletThings)
		USoundBase* BulletFireSound = nullptr; 

	int score; 

	//for week 6
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
			UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

};
