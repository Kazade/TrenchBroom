/*
 Copyright (C) 2010-2016 Kristian Duske
 
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

#ifndef TrenchBroom_NodeReader
#define TrenchBroom_NodeReader

#include "IO/MapReader.h"
#include "Model/ModelTypes.h"

namespace TrenchBroom {
    namespace Model {
        class ModelFactory;
    }
    
    namespace IO {
        class ParserStatus;
        
        class NodeReader : public MapReader {
        private:
            Model::ModelFactory* m_factory;
            Model::NodeList m_nodes;
        public:
            NodeReader(const String& str, Model::ModelFactory* factory);
            
            static Model::NodeList read(const String& str, Model::ModelFactory* factory, const BBox3& worldBounds, ParserStatus& status);
            const Model::NodeList& read(const BBox3& worldBounds, ParserStatus& status);
        private: // implement MapReader interface
            Model::ModelFactory* initialize(Model::MapFormat::Type format, const BBox3& worldBounds);
            Model::Node* onWorldspawn(const Model::EntityAttribute::List& attributes, const ExtraAttributes& extraAttributes, ParserStatus& status);
            void onWorldspawnFilePosition(size_t lineNumber, size_t lineCount, ParserStatus& status);
            void onLayer(Model::Layer* layer, ParserStatus& status);
            void onNode(Model::Node* parent, Model::Node* node, ParserStatus& status);
            void onUnresolvedNode(const ParentInfo& parentInfo, Model::Node* node, ParserStatus& status);
            void onBrush(Model::Node* parent, Model::Brush* brush, ParserStatus& status);
        };
    }
}

#endif /* defined(TrenchBroom_NodeReader) */
