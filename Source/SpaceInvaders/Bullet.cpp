// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(Collision);

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
	TimeLived += DeltaTime; 


	if (TimeLived > TimeBeforeDestroy)
	{
		this->Destroy(); 
	}

}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == this)
		return;

	UE_LOG(LogTemp, Warning, TEXT("%s overlapping with %s at location %s"), *GetName(), 
		*OtherActor->GetName(),
		*GetActorLocation().ToString()); 

	OnBulletHitEnemy.Broadcast(OtherActor);
}

