#pragma once

#include "Editor.h"
#include "LightingBuildOptions.h"


class FSpawnManager
{
public:
   FSpawnManager();

protected:
   void InitializeScene() const;
   void ClearAllActorsOfClass() const;
   void SpawnNavMesh() const;
   void SpawnGrids() const;

private:
   FLightingBuildOptions LightOptions;
   UWorld* World = GEditor->GetEditorWorldContext().World();
};
