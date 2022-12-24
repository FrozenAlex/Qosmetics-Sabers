#pragma once

#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "custom-types/shared/macros.hpp"

#include "CustomTypes/TrailHandler.hpp"
#include "CustomTypes/TrailTransform.hpp"
#include "CustomTypes/WhackerColorHandler.hpp"

#include "private_field.hpp"

DECLARE_CLASS_CODEGEN(Qosmetics::Sabers, WhackerHandler, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_PRIVATE_FIELD(WhackerColorHandler*, colorHandler);
                      DECLARE_INSTANCE_PRIVATE_FIELD(ArrayW<TrailHandler*>, trailHandlers);
                      DECLARE_INSTANCE_PRIVATE_FIELD(ArrayW<TrailTransform*>, trailTransforms);
                      DECLARE_INSTANCE_PRIVATE_FIELD(GlobalNamespace::SaberModelController*, saberModelController);
                      DECLARE_INSTANCE_PRIVATE_FIELD(GlobalNamespace::SaberType, saberType);

                      DECLARE_INSTANCE_METHOD(void, GetTrailHandlers);
                      DECLARE_INSTANCE_METHOD(void, SetupTrails);
                      DECLARE_SIMPLE_DTOR();

                      public
                      :

                      void SetColor(const Sombrero::FastColor& thisColor, const Sombrero::FastColor& thatColor);
                      void SetSize(float width, float length);

)