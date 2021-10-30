#pragma once
#include <string>

extern const std::string VISIBLE;
extern const std::string OPACITY;
extern const std::string IS_CLIP;
extern const std::string ANGLE;
extern const std::string CENTER_POINT;
extern const std::string FLIP;
 
extern const std::string FORE_GROUND_COLOR;
extern const std::string BACK_GROUND_COLOR;
extern const std::string IS_BROADCAST_EVENT;
 
extern const std::string X_COORDINATOR;
extern const std::string Y_COORDINATOR;
extern const std::string Z_COORDINATOR;

extern const std::string TRANS_VECTOR;
extern const std::string ROTATE_VECTOR;
extern const std::string SCALE_VECTOR;

extern const std::string LAYOUT_WIDTH;
extern const std::string LAYOUT_HEIGHT;
 
extern const std::string SCALE_X;
extern const std::string SCALE_Y;
 
extern const std::string TEXT;
extern const std::string FONT_NAME;
extern const std::string FONT_SIZE;
extern const std::string FONT_COLOR;

extern const std::string BLEND_INDENSITY;
extern const std::string CAM_POSITION;
extern const std::string CAM_FRONT;
extern const std::string CAM_UP;
extern const std::string CAM_TARGET;
extern const std::string CAM_PITCH;
extern const std::string CAM_YAW;
extern const std::string CAM_SPEED;
extern const std::string FOV;
extern const std::string NEAR_PLANE;
extern const std::string FAR_PLANE;
extern const std::string CAMERA_TYPE;

#define IS_VALID_COLOR(color)		(0 != color.r && 0 != color.g && 0 != color.b && 0 != color.a)
