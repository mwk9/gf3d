#ifndef __GF3D_CAMERA_H__
#define __GF3D_CAMERA_H__

#include <math.h>
#include "gf3d_matrix.h"
#include "gf3d_vgraphics.h"
#include "uniforms.h"
#include "simple_logger.h"

typedef struct camera_s
{
	UniformBufferObject *ubo;
	Vector3D worldUp;

	float width;
	float height;

	Vector3D front;
	Vector3D up;
	Vector3D right;
	Vector3D *position;
	Vector3D *rotation;

	float fieldOfView;
	float nearPlane;
	float farPlane;
}Camera;

Camera *gf3d_camera_init(int renderWidth, int renderHeight, Vector3D *position, Vector3D *rotation);

/**
 * @brief get the current camera view
 * @param view output, the matrix provided will be populated with the current camera information
 */
void gf3d_camera_get_view(Matrix4 view);

/**
 * @brief set the current camera based on the matrix provided
 */
void gf3d_camera_set_view(Matrix4 view);

/**
 * @brief set the camera properties based on position and direction that the camera should be looking
 * @param position the location for the camera
 * @param target the point the camera should be looking at
 * @param up the direction considered to be "up"
 */
void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
);

/**
 * @brief explicitely set the camera positon, holding all other parameters the same
 * @param position the new position for the camera
 */
void gf3d_camera_set_position(Vector3D position);

/**
 * @brief move the camera relatively based on the vector provided
 * @param move the ammount to move the camera
 */
//void gf3d_camera_move(Vector3D move);
void gf3d_camera_move(Camera *camera, float deltaForwardScale, float deltaRightScale, float deltaUpScale);
void gf3d_camera_turn(Camera *camera, float deltaPitch, float deltaYaw, float deltaRoll);
void gf3d_camera_update(Camera *camera);

#endif
