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

#ifndef __TrenchBroom__FlyModeHelper__
#define __TrenchBroom__FlyModeHelper__

#include "VecMath.h"

#include <iostream>

#include <wx/wx.h>

namespace TrenchBroom {
    namespace View {
        class FlyModeHelper {
        public:
            struct Input {
                bool forward;
                bool backward;
                bool left;
                bool right;
                wxPoint delta;
                unsigned int time;
                
                Input();
            };
        private:
            wxWindow* m_window;
            
            bool m_forward;
            bool m_backward;
            bool m_left;
            bool m_right;
            
            bool m_enabled;
            
            wxPoint m_originalMousePos;
            wxLongLong m_lastPollTime;
        public:
            FlyModeHelper(wxWindow* window);
            ~FlyModeHelper();
            
            void enable();
            void disable();
            bool enabled() const;
            
            Input poll();
        private:
            void lockMouse();
            void unlockMouse();

            wxPoint mouseDelta() const;
            void resetMouse();
            
            wxPoint windowCenter() const;
            
            void OnKeyDown(wxKeyEvent& event);
            void OnKeyUp(wxKeyEvent& event);
            void onKey(wxKeyEvent& event, bool down);
        };
    }
}

#endif /* defined(__TrenchBroom__FlyModeHelper__) */