// Original by:
// (c) 2018, Steinberg Media Technologies GmbH, All Rights Reserved
//
// Modified by:
// (c) 2020 Takamitsu Endo
//
// This file is part of UhhyouDebug.
//
// UhhyouDebug is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// UhhyouDebug is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with UhhyouDebug.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "base/source/fstreamer.h"
#include "base/source/fstring.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
#include "public.sdk/source/vst/vstparameters.h"

#include "logger/logger.hpp"

#include <algorithm>
#include <vector>

namespace Steinberg {
namespace Synth {

template<typename EditorType, typename ParameterType>
class PlugController : public Vst::EditController, public Vst::IMidiMapping {
public:
  std::vector<EditorType *> editor;

  ~PlugController()
  {
    for (auto &edi : editor) edi->forget();
  }

  static FUnknown *createInstance(void *)
  {
    Logger::write("PlugController::createInstance() is called");

    auto value = (Vst::IEditController *)new PlugController();

    Logger::write("PlugController::createInstance(): Exiting");

    return value;
  }

  tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
  tresult PLUGIN_API setComponentState(IBStream *state) SMTG_OVERRIDE;
  IPlugView *PLUGIN_API createView(const char *name) SMTG_OVERRIDE;
  void editorDestroyed(Vst::EditorView *editorView) SMTG_OVERRIDE;
  tresult PLUGIN_API setParamNormalized(Vst::ParamID id, Vst::ParamValue normalized)
    SMTG_OVERRIDE;

  tresult PLUGIN_API getMidiControllerAssignment(
    int32 busIndex, int16 channel, Vst::CtrlNumber midiControllerNumber, Vst::ParamID &id)
    SMTG_OVERRIDE;

  OBJ_METHODS(PlugController, EditController)
  DEFINE_INTERFACES
  DEF_INTERFACE(IMidiMapping)
  END_DEFINE_INTERFACES(EditController)
  REFCOUNT_METHODS(EditController)
};

template<typename EditorType, typename ParameterType>
tresult PLUGIN_API
PlugController<EditorType, ParameterType>::initialize(FUnknown *context)
{
  Logger::write("PlugController::initialize() is called");

  if (EditController::initialize(context) != kResultTrue) return kResultTrue;
  ParameterType param;
  auto result = param.addParameter(parameters);

  Logger::write("PlugController::initialize(): Exiting");

  return result;
}

template<typename EditorType, typename ParameterType>
tresult PLUGIN_API
PlugController<EditorType, ParameterType>::setComponentState(IBStream *state)
{
  Logger::write("PlugController::setComponentState() is called");

  if (!state) return kResultFalse;

  ParameterType param;
  if (param.setState(state)) return kResultFalse;

  for (auto &val : param.value)
    if (setParamNormalized(val->getId(), val->getNormalized())) return kResultFalse;

  Logger::write("PlugController::setComponentState(): Exiting");

  return kResultOk;
}

template<typename EditorType, typename ParameterType>
IPlugView *PLUGIN_API
PlugController<EditorType, ParameterType>::createView(const char *name)
{
  Logger::write("PlugController::createView() is called");

  if (name && strcmp(name, "editor") == 0) {
    auto plugEditor = new EditorType(this);
    plugEditor->remember();
    editor.push_back(plugEditor);

    Logger::write("PlugController::createView(): editor is created");

    return plugEditor;
  }

  Logger::write("PlugController::createView(): name is not editor");

  return 0;
}

template<typename EditorType, typename ParameterType>
void PlugController<EditorType, ParameterType>::editorDestroyed(
  Vst::EditorView *editorView)
{
  Logger::write("PlugController::editorDestroyed() is called");

  auto iter = std::find(editor.begin(), editor.end(), editorView);
  if (iter == editor.end()) {
    Logger::write(
      "PlugController::editorDestroyed(): editorView doesn't match existing view");
    return;
  }
  (*iter)->forget();
  editor.erase(iter);

  Logger::write("PlugController::editorDestroyed(): Exiting");
}

template<typename EditorType, typename ParameterType>
tresult PLUGIN_API PlugController<EditorType, ParameterType>::setParamNormalized(
  Vst::ParamID id, Vst::ParamValue normalized)
{
  Logger::write("PlugController::setParamNormalized() is called");

  Vst::Parameter *parameter = getParameterObject(id);
  if (parameter) {
    parameter->setNormalized(normalized);
    for (auto &edi : editor) edi->updateUI(id, normalized);

    Logger::write("PlugController::setParamNormalized(): parameter is set");

    return kResultTrue;
  }

  Logger::write("PlugController::setParamNormalized(): parameter is empty");

  return kResultFalse;
}

} // namespace Synth
} // namespace Steinberg
