/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2021 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/
#pragma once

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/processors/canvasprocessorwidget.h>
#include <inviwo/core/processors/processor.h>

#include <modules/openglqt/canvasqopenglwidget.h>

#include <functional>
#include <vector>
#include <string>

#include <warn/push>
#include <warn/ignore/all>
#include <QMainWindow>
#include <warn/pop>

class QResizeEvent;
class QShowEvent;
class QHideEvent;
class QMoveEvent;

namespace inviwo {

class Processor;
class PropertyListFrame;

class IVW_MODULE_OPENGLQT_API CanvasWithPropertiesProcessorWidgetQt : public CanvasProcessorWidget,
                                                                      public QMainWindow {
public:
    CanvasWithPropertiesProcessorWidgetQt(Processor* p);
    virtual ~CanvasWithPropertiesProcessorWidgetQt() = default;

    // CanvasProcessorWidget overrides
    virtual Canvas* getCanvas() const override;

    // ProcessorWidget overrides
    virtual void setVisible(bool visible) override;
    virtual void setDimensions(ivec2 dimensions) override;
    virtual void setPosition(ivec2 position) override;
    virtual void setFullScreen(bool fullScreen) override;
    virtual void setOnTop(bool onTop) override;

    // QMainWindow overrides
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void showEvent(QShowEvent*) override;
    virtual void hideEvent(QHideEvent*) override;
    virtual void moveEvent(QMoveEvent*) override;
    virtual void changeEvent(QEvent*) override;

    void addProperties(std::string_view paths);

protected:
    using Super = QMainWindow;

    virtual void propagateResizeEvent() override;

    bool contextMenu(QMenu& menu);
    
    // ProcessorWidget overrides
    virtual void updateVisible(bool visible) override;
    virtual void updateDimensions(ivec2) override;
    virtual void updatePosition(ivec2) override;
    virtual void updateFullScreen(bool) override;
    virtual void updateOnTop(bool) override;

    std::unique_ptr<CanvasQOpenGLWidget, std::function<void(CanvasQOpenGLWidget*)>> canvas_;
    Processor::NameDispatcherHandle nameChange_;

    size2_t canvasDimensions_{0};
    PropertyListFrame* frame_;
    std::vector<std::string> addedPaths_;
};

}  // namespace inviwo
