/*
 Copyright (C) 2010-2013 Kristian Duske
 
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
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "TextureManager.h"

namespace TrenchBroom {
    namespace Model {
        void TextureManager::addTextureCollection(const IO::Path& path) {
            assert(m_game != NULL);
            doAddTextureCollection(path, m_collections, m_collectionsByPath, m_toUpload, m_toRemove);
            updateTextures();
        }
        
        void TextureManager::addTextureCollections(const IO::Path::List& paths) {
            assert(m_game != NULL);

            if (paths.empty())
                return;
            
            TextureCollection::List collections;
            TextureCollectionMap collectionsByPath;
            TextureCollectionMap toUpload;
            TextureCollectionMap toRemove = m_toRemove;
            
            IO::Path::List::const_iterator it, end;
            for (it = paths.begin(), end = paths.end(); it != end; ++it) {
                const IO::Path& path = *it;
                doAddTextureCollection(path, collections, collectionsByPath, toUpload, toRemove);
            }
            
            m_collections.insert(m_collections.end(), collections.begin(), collections.end());
            m_collectionsByPath.insert(collectionsByPath.begin(), collectionsByPath.end());
            m_toUpload.insert(toUpload.begin(), toUpload.end());
            m_toRemove = toRemove;

            updateTextures();
        }

        void TextureManager::removeTextureCollection(const size_t index) {
            assert(index < m_collections.size());
            
            TextureCollection::List::iterator it = m_collections.begin() + index;
            TextureCollection::Ptr collection = *it;
            
            m_collections.erase(it);
            m_collectionsByPath.erase(collection->path());
            m_toUpload.erase(collection->path());
            m_toRemove.insert(TextureCollectionMapEntry(collection->path(), collection));
            updateTextures();
        }
        
        void TextureManager::reset(Game::Ptr game) {
            m_toRemove.insert(m_collectionsByPath.begin(), m_collectionsByPath.end());
            m_collections.clear();
            m_collectionsByPath.clear();
            m_toUpload.clear();
            m_game = game;
            updateTextures();
        }
        
        void TextureManager::commitChanges() {
            if (m_game != NULL) {
                TextureCollectionMap::iterator it, end;
                for (it = m_toUpload.begin(), end = m_toUpload.end(); it != end; ++it) {
                    TextureCollection::Ptr collection = it->second;
                    m_game->uploadTextureCollection(collection);
                }
                
                m_toUpload.clear();
                m_toRemove.clear();
            }
        }

        Texture::Ptr TextureManager::texture(const String& name) const {
            TextureMap::const_iterator it = m_texturesByName.find(name);
            if (it == m_texturesByName.end())
                return Texture::Ptr();
            return it->second;
        }

        void TextureManager::doAddTextureCollection(const IO::Path& path, TextureCollection::List& collections, TextureCollectionMap& collectionsByPath, TextureCollectionMap& toUpload, TextureCollectionMap& toRemove) const {
            if (collectionsByPath.count(path) > 0)
                return;
            
            TextureCollectionMap::iterator it = toRemove.find(path);
            if (it != toRemove.end()) {
                TextureCollection::Ptr collection = it->second;
                toRemove.erase(it);
                doAddTextureCollection(collection, collections, collectionsByPath, toUpload);
            } else {
                TextureCollection::Ptr collection = m_game->loadTextureCollection(path);
                doAddTextureCollection(collection, collections, collectionsByPath, toUpload);
            }
        }

        void TextureManager::doAddTextureCollection(TextureCollection::Ptr collection, TextureCollection::List& collections, TextureCollectionMap& collectionsByPath, TextureCollectionMap& toUpload) const {
            collectionsByPath.insert(TextureCollectionMapEntry(collection->path(), collection));
            collections.push_back(collection);
            toUpload.insert(TextureCollectionMapEntry(collection->path(), collection));
        }

        void TextureManager::updateTextures() {
            m_texturesByName.clear();
            
            TextureCollection::List::const_iterator cIt, cEnd;
            for (cIt = m_collections.begin(), cEnd = m_collections.end(); cIt != cEnd; ++cIt) {
                TextureCollection::Ptr collection = *cIt;
                const Texture::List textures = collection->textures();
                
                Texture::List::const_iterator tIt, tEnd;
                for (tIt = textures.begin(), tEnd = textures.end(); tIt != tEnd; ++tIt) {
                    Texture::Ptr texture = *tIt;
                    m_texturesByName[texture->name()] = texture;
                }
            }
        }
    }
}