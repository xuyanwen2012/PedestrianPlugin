#include "SpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Atmosphere/AtmosphericFog.h"

#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "NavMeshBoundsVolume.h"
#include "Builders/CubeBuilder.h"

FSpawnManager::FSpawnManager()
{
   ClearAllActorsOfClass();
   InitializeScene();
   SpawnGrids();
}

void FSpawnManager::InitializeScene() const
{
   FVector Location{ 0.0f, 0.0f, 2000.0f };
   const FRotator Rotation{};
   const FActorSpawnParameters SpawnParameters;

   World->SpawnActor<AAtmosphericFog>(Location, Rotation, SpawnParameters);
   Location.Z = 100000.0f;

   const auto Skylight = World->SpawnActor<ASkyLight>(Location, Rotation, SpawnParameters);

   if (Skylight != nullptr) {
      Skylight->GetLightComponent()->SetIntensity(5.0f);
      GEditor->BuildLighting(LightOptions);
   }
}

void FSpawnManager::ClearAllActorsOfClass() const
{
   TArray<AActor*> FoundActors;

   UGameplayStatics::GetAllActorsOfClass(World, AAtmosphericFog::StaticClass(), FoundActors);

   for (auto FoundActor : FoundActors)
   {
      World->DestroyActor(FoundActor);
   }

   UGameplayStatics::GetAllActorsOfClass(World, ASkyLight::StaticClass(), FoundActors);

   for (auto FoundActor : FoundActors)
   {
      World->DestroyActor(FoundActor);
   }
}

void FSpawnManager::SpawnNavMesh() const
{

   UCubeBuilder* CubeBuilder = Cast<UCubeBuilder>(GEditor->FindBrushBuilder(UCubeBuilder::StaticClass()));
   CubeBuilder->X = 1000;
   CubeBuilder->Y = 1000;
   CubeBuilder->Z = 200;
   CubeBuilder->Build(World);

   auto MeshBoundsVolume = World->SpawnActor<ANavMeshBoundsVolume>(FVector{}, FRotator{});


   // //Spawn Navmesh
   //auto MeshBoundsVolume = World->SpawnActorDeferred<ANavMeshBoundsVolume>(
   //   ANavMeshBoundsVolume::StaticClass(), 
   //   FTransform{});

   // hack-change modify frequency - otherwise SetActorLocation will fail.
   MeshBoundsVolume->GetRootComponent()->SetMobility(EComponentMobility::Stationary);

   MeshBoundsVolume->SetActorLocation(FVector::ZeroVector, false);
   MeshBoundsVolume->SetActorRelativeScale3D(FVector{ 20.0f, 20.0f, 20.0f });
   //MeshBoundsVolume->SetActorLocation(MyBounds.GetCenter(), false);
   //MeshBoundsVolume->SetActorRelativeScale3D(MyBounds.GetSize());
   MeshBoundsVolume->GetRootComponent()->UpdateBounds();

   // redo modify frequency change
   MeshBoundsVolume->GetRootComponent()->SetMobility(EComponentMobility::Static);

   //MeshBoundsVolume->BrushBuilder = CubeBuilder;

   //UGameplayStatics::FinishSpawningActor(MeshBoundsVolume, FTransform{});

   MeshBoundsVolume->RebuildNavigationData();
}

void FSpawnManager::SpawnGrids() const
{

}
