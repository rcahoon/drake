#include "drake/planning/robot_diagram_builder.h"

#include <stdexcept>
#include <vector>

#include "drake/common/drake_throw.h"

namespace drake {
namespace planning {

using geometry::SceneGraph;
using multibody::AddMultibodyPlantSceneGraph;
using systems::DiagramBuilder;
using systems::System;

template <typename T>
RobotDiagramBuilder<T>::RobotDiagramBuilder(double time_step)
    : builder_(std::make_unique<DiagramBuilder<T>>()),
      pair_(AddMultibodyPlantSceneGraph<T>(builder_.get(), time_step)),
      plant_(pair_.plant),
      scene_graph_(pair_.scene_graph),
      parser_(&plant_) {}

template <typename T>
RobotDiagramBuilder<T>::~RobotDiagramBuilder() = default;

template <typename T>
std::unique_ptr<RobotDiagram<T>> RobotDiagramBuilder<T>::Build() {
  ThrowIfAlreadyBuiltOrCorrupted();
  if (!plant().is_finalized()) {
    plant().Finalize();
  }
  return std::unique_ptr<RobotDiagram<T>>(
      new RobotDiagram<T>(std::move(builder_)));
}

template <typename T>
bool RobotDiagramBuilder<T>::IsDiagramBuilt() const {
  if (builder_ == nullptr) {
    return true;
  }
  if (builder_->already_built()) {
    throw std::logic_error(
        "RobotDiagramBuilder: Do not call mutable_builder().Build() to create"
        " a Diagram; instead, call Build() to create a RobotDiagram.");
  }
  return false;
}

template <typename T>
void RobotDiagramBuilder<T>::ThrowIfAlreadyBuiltOrCorrupted() const {
  if (IsDiagramBuilt()) {
    throw std::logic_error(
        "RobotDiagramBuilder: Build() has already been called to create a"
        " RobotDiagram; this RobotDiagramBuilder may no longer be used.");
  }
  // Check that the user didn't remove (delete) the plant or scene graph.
  std::vector<const System<T>*> systems = builder_->GetSystems();
  const bool is_good = systems.size() >= 2 && systems[0] == &plant_ &&
                       systems[1] == &scene_graph_;
  if (!is_good) {
    throw std::logic_error(
        "RobotDiagramBuilder: The underlying DiagramBuilder has become "
        "corrupted. You must not remove the MultibodyPlant or SceneGraph.");
  }
}

}  // namespace planning
}  // namespace drake

DRAKE_DEFINE_CLASS_TEMPLATE_INSTANTIATIONS_ON_DEFAULT_SCALARS(
    class ::drake::planning::RobotDiagramBuilder)
