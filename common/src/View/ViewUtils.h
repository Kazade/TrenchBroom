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

#ifndef __TrenchBroom__ViewUtils__
#define __TrenchBroom__ViewUtils__

#include "View/ViewTypes.h"

#include <wx/wx.h>

namespace TrenchBroom {
    class Logger;
    
    namespace Assets {
        class EntityModel;
        class EntityModelManager;
        struct ModelSpecification;
    }
    
    namespace View {
        Assets::EntityModel* safeGetModel(Assets::EntityModelManager& manager, const Assets::ModelSpecification& spec, Logger& logger);
        void combineFlags(const size_t numFlags, const int newFlagValue, int& setFlags, int& mixedFlags);
        
        bool loadTextureCollection(MapDocumentWPtr document, ControllerWPtr controller, wxWindow* parent, const wxString& wxPath);
        size_t loadTextureCollections(MapDocumentWPtr document, ControllerWPtr controller, wxWindow* parent, const wxArrayString& wxPaths);
        
        bool loadEntityDefinitionFile(MapDocumentWPtr document, ControllerWPtr controller, wxWindow* parent, const wxString& wxPath);
        size_t loadEntityDefinitionFile(MapDocumentWPtr document, ControllerWPtr controller, wxWindow* parent, const wxArrayString& wxPath);
    }
}

#endif /* defined(__TrenchBroom__ViewUtils__) */
