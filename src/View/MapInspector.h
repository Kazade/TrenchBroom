/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__MapInspector__
#define __TrenchBroom__MapInspector__

#include "View/ViewTypes.h"

#include <wx/panel.h>

class wxCollapsiblePaneEvent;
class wxWindow;

namespace TrenchBroom {
    namespace Renderer {
        class Camera;
        class RenderResources;
    }
    
    namespace View {
        class MapTreeView;
        class ModEditor;
        
        class MapInspector : public wxPanel {
        public:
            MapInspector(wxWindow* parent, MapDocumentWPtr document, ControllerWPtr controller, Renderer::RenderResources& resources, Renderer::Camera& camera);

            void OnPaneChanged(wxCollapsiblePaneEvent& event);
        private:
            void createGui(MapDocumentWPtr document, ControllerWPtr controller, Renderer::RenderResources& resources, Renderer::Camera& camera);
            wxWindow* createMiniMap(wxWindow* parent, MapDocumentWPtr document, Renderer::RenderResources& resources, Renderer::Camera& camera);
            wxWindow* createMapTree(wxWindow* parent, MapDocumentWPtr document, ControllerWPtr controller);
            wxWindow* createModEditor(wxWindow* parent, MapDocumentWPtr document, ControllerWPtr controller);
        };
    }
}

#endif /* defined(__TrenchBroom__MapInspector__) */