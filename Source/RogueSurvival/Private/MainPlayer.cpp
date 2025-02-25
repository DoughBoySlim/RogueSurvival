// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Setting Up the Movement of the characters
void AMainPlayer::OnMoveToClick()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		FHitResult HitResult;

		if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			ClickLocation = HitResult.Location;
			

			MainDirection = ClickLocation - PlayerLocation;
			MainDirection.Normalize();
		}
	}
}

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating the Camera
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1000.0f;
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->SetWorldRotation(FRotator(-60.0f, 0.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Attaching the Static Mesh to the Player
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerBodyMesh(TEXT("StaticMesh'/Game/Fab/Human_body_base__cartoon_/SM_mainPlayerBody.SM_mainPlayerBody'"));
	if (PlayerBodyMesh.Succeeded()) { StaticMeshComponent->SetStaticMesh(PlayerBodyMesh.Object); }

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get Player Location
	PlayerLocation = GetActorLocation();
	// Calculating the distance between the player and the click
	float Distance = FVector::Dist(PlayerLocation, ClickLocation);

	UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *PlayerLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Click Location: %s"), *ClickLocation.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), Distance);

	if (!MainDirection.IsZero()) {
		AddMovementInput(MainDirection, MovementSpeed * DeltaTime);
	}
	if (Distance <= 1.0f) {
		MainDirection = FVector::ZeroVector;
		MovementSpeed = 0.0f;
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("MoveToClick", IE_Pressed, this, &AMainPlayer::OnMoveToClick);
}



