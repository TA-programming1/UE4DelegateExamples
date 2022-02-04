// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShip.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerInput.h"
#include "Camera/CameraActor.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"  
#include "GameFramework/Character.h"
// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));

	SetRootComponent(PlayerMesh); 
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); 
	SpringArm->bDoCollisionTest = false; 
	SpringArm->SetUsingAbsoluteRotation(true); 
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f)); 
	SpringArm->TargetArmLength = 1200.f; 
	SpringArm->bEnableCameraLag = false; 
	SpringArm->CameraLagSpeed = 5.f; 
	SpringArm->SetupAttachment(PlayerMesh); 


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false; 
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); 
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	InitLocation = PlayerMesh->GetComponentLocation();
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InContact = false; 
	PlayerMesh->AddRelativeLocation(FVector(XValue, YValue, 0.f) * PlayerSpeed);
}

static void InitializeDefaultPawnInputBinding()
{
	static bool bindingsAdded = false;

	if (bindingsAdded == false)
	{
		bindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveX", EKeys:: W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveX", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveY", EKeys::D, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveY", EKeys::A, -1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Shoot", EKeys::SpaceBar));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Reload", EKeys::R));
	}
}


// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent != nullptr)
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InitializeDefaultPawnInputBinding();
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerShip::MoveXAxis);  
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerShip::MoveYAxis);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &APlayerShip::Shoot);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &APlayerShip::Reload);

}


void APlayerShip::ResetLocation() const
{

}

void APlayerShip::OnEnemyHit(AActor* Enemy)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEnemyHit successfully broadcast to playership"));
}


void APlayerShip::Shoot()
{
	if (Ammo > 0)
	{
		Ammo--;
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White, FString::Printf(TEXT("Ammo: %d"), Ammo));


		UWorld* World = GetWorld();
		if (World)
		{
			FVector Location = GetActorLocation();

			ABullet* Bullet = World->SpawnActor<ABullet>(ActorToSpawn, Location + FVector(150.f, 0.f, 0.f), GetActorRotation());
			UGameplayStatics::PlaySound2D(World, ShootingSound, 1.f, 1.f, 0.f, 0);

			if (Bullet)
			{
				Bullet->OnBulletHitEnemy.AddDynamic(this, &APlayerShip::OnEnemyHit);
			}
		}

		if (Ammo == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Red, FString::Printf(TEXT("No ammo reload: %d"), Ammo));
		}
	}
}


void APlayerShip::Reload()
{
	Ammo = 30;

	UWorld* NewWorld = GetWorld();
	UGameplayStatics::PlaySound2D(NewWorld, ReloadingSound, 1.f, 1.f, 0.f, 0); 
	GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::Green, FString::Printf(TEXT("Reloaded %d "), Ammo)); 

}

void APlayerShip::MoveXAxis(float Value)
{
	XValue = Value;
}

void APlayerShip::MoveYAxis(float Value)
{
	YValue = Value; 
}

void APlayerShip::Dash()
{

}

