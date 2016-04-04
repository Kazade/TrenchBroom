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

#ifndef MapCompilationProfile_h
#define MapCompilationProfile_h

#include "View/MapCompilationTask.h"

namespace TrenchBroom {
    namespace View {
        class MapCompilationContext;

        class MapCompilationProfileRunner {
        private:
            MapCompilationTask::TaskRunner* m_tasks;
        public:
            MapCompilationProfileRunner(MapCompilationContext& context, const MapCompilationTask::List& tasks);
            ~MapCompilationProfileRunner();
            
            void execute();
            void terminate();
        private:
            MapCompilationProfileRunner(const MapCompilationProfileRunner& other);
            MapCompilationProfileRunner& operator=(const MapCompilationProfileRunner& other);
        };
        
        class MapCompilationProfile {
        private:
            MapCompilationTask::List m_tasks;
        public:
            MapCompilationProfile();
            ~MapCompilationProfile();

            MapCompilationProfileRunner* createRunner(MapCompilationContext& context) const;
        private:
            MapCompilationProfile(const MapCompilationProfile& other);
            MapCompilationProfile& operator=(const MapCompilationProfile& other);
        };
    }
}

#endif /* MapCompilationProfile_h */