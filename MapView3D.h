//
//  MapView3D.h
//  TrenchBroom
//
//  Created by Kristian Duske on 15.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

extern NSString* const MapView3DDefaults;
extern NSString* const MapView3DDefaultsBackgroundColor;

@class TextureManager;
@class VBOBuffer;
@class InputManager;
@class Camera;
@class RenderMap;
@class SelectionManager;

@interface MapView3D : NSOpenGLView {
    @private
    float backgroundColor[3];
    RenderMap* renderMap;
    Camera* camera;
    TextureManager* textureManager;
    InputManager* inputManager;
    SelectionManager* selectionManager;
    VBOBuffer* faceVBO;
    VBOBuffer* edgeVBO;
}

- (void)setCamera:(Camera *)aCamera;
- (void)setRenderMap:(RenderMap *)aRenderMap;
- (void)setFaceVBO:(VBOBuffer *)theFaceVBO;
- (void)setEdgeVBO:(VBOBuffer *)theEdgeVBO;
- (void)setTextureManager:(TextureManager *)theTextureManager;
- (void)setInputManager:(InputManager *)theInputManager;
- (void)setSelectionManager:(SelectionManager *)theSelectionManager;

- (Camera *)camera;

- (void)userDefaultsChanged:(NSNotification *)notification;

@end
