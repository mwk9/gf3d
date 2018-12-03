#include "gf3d_matrix.h"
#include "gf3d_camera.h"

#include <string.h>

Matrix4 gf3d_camera = {0};

static float gf3d_camera_aspect_ratio(Camera *camera)
{
	if (!camera)
	{
		return -1.0f;
	}
	return camera->width / camera->height;
}

static float gf3d_camera_get_pitch(Camera *camera)
{
	if (!camera)
	{
		return -1;
	}
	return camera->rotation->x;
}

static float gf3d_camera_get_yaw(Camera *camera)
{
	if (!camera)
	{
		return -1;
	}
	return camera->rotation->y;
}

static float gf3d_camera_get_roll(Camera *camera)
{
	if (!camera)
	{
		return -1;
	}
	return camera->rotation->z;
}

void gf3d_camera_get_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(view,gf3d_camera,sizeof(Matrix4));
}

void gf3d_camera_set_view(Matrix4 *view)
{
    if (!view)return;
    memcpy(gf3d_camera,view,sizeof(Matrix4));
}

void gf3d_camera_look_at(
    Vector3D position,
    Vector3D target,
    Vector3D up
)
{
    gf3d_matrix_view(
        gf3d_camera,
        position,
        target,
        up
    );
}

void gf3d_camera_set_position(Vector3D position)
{
    gf3d_camera[0][3] = position.x;
    gf3d_camera[1][3] = position.y;
    gf3d_camera[2][3] = position.z;
}

/*void gf3d_camera_move(Vector3D move)
{
    gf3d_camera[0][3] += move.x;
    gf3d_camera[1][3] += move.y;
    gf3d_camera[2][3] += move.z;
}*/

static void gf3d_camera_set_pitch(Camera *camera, float pitch)
{
	if (!camera)
	{
		return;
	}

	//hardcoded to avoid gimble locking
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	camera->rotation->x = pitch;
}

static void gf3d_camera_set_yaw(Camera *camera, float yaw)
{
	float _yaw = 0.0f;

	if (!camera)
	{
		return;
	}

	_yaw = fmodf(yaw, 360.0f);
	if (_yaw < 0.0f)
	{
		_yaw += 360.0f;
	}
	camera->rotation->y = _yaw;
}

static void gf3d_camera_set_roll(Camera *camera, float roll)
{
	float _roll = 0.0f;

	if (!camera)
	{
		return;
	}

	//still needs roll
	//_roll = fmodf(roll, )
}

void gf3d_camera_turn(Camera *camera, float deltaPitch, float deltaYaw, float deltaRoll)
{
	gf3d_camera_set_pitch(camera, gf3d_camera_get_pitch(camera) + deltaPitch);
	gf3d_camera_set_yaw(camera, gf3d_camera_get_yaw(camera) + deltaYaw);
	//gf3d_camera_set_roll(camera, gf3d_camera_get_roll(camera) + deltaRoll);
}

void gf3d_camera_move(Camera *camera, float deltaForwardScale, float deltaRightScale, float deltaUpScale)
{
	if (!camera)
	{
		return;
	}

	Vector3D deltaForward, deltaRight, deltaUp;

	vector3d_scale(deltaForward, camera->front, deltaForwardScale);
	vector3d_scale(deltaRight, camera->right, deltaRightScale);
	vector3d_scale(deltaUp, camera->up, deltaUpScale);

	Vector3D_add(camera->position, camera->position, &deltaForward);
	Vector3D_add(camera->position, camera->position, &deltaRight);
	Vector3D_add(camera->position, camera->position, &deltaUp);
}

void gf3d_camera_vector_front_update(Camera *camera)
{
	if (!camera)
	{
		return;
	}

	float pitch = DEGTORAD(gf3d_camera_get_pitch(camera));
	float yaw = DEGTORAD(gf3d_camera_get_yaw(camera));
	camera->front.x = cosf(yaw) * cosf(pitch);
	camera->front.y = sinf(pitch);
	camera->front.z = sinf(yaw) * cosf(pitch);
	vector3d_normalize(&camera->front);
}

void gf3d_camera_vector_right_update(Camera *camera)
{
	if (!camera)
	{
		return;
	}

	vector3d_cross_product(&camera->right, camera->right, camera->worldUp);
	vector3d_normalize(&camera->right);
}

void gf3d_camera_vector_up_update(Camera *camera)
{
	if (!camera)
	{
		return;
	}

	vector3d_cross_product(&camera->up, camera->right, camera->front);
	vector3d_normalize(&camera->up);
}

void gf3d_camera_update(Camera *camera)
{
	static Vector3D target;

	if (!camera)
	{
		return;
	}

	gf3d_camera_vector_front_update(camera);
	gf3d_camera_vector_right_update(camera);
	gf3d_camera_vector_up_update(camera);
	

	Vector3D_add(&target, camera->position, &camera->front);
	//gf3d_matrix_lookat(camera->ubo->view, *camera->position, target, camera->up);
	gf3d_matrix_view(camera->ubo->view, *camera->position, target, camera->up);
	//gf3d_matrix_perspective_vec3(camera->ubo->proj, camera->fieldOfView, gf3d_camera_aspect_ratio(camera), camera->nearPlane, camera->farPlane);
	gf3d_matrix_perspective(camera->ubo->proj, camera->fieldOfView, gf3d_camera_aspect_ratio(camera), camera->nearPlane, camera->farPlane);
	camera->ubo->proj[0][0] *= -1.0f;

	//gf3d_matrix_view(camera->view, camera->position, camera->target, vector3d(0, 0, 1));
	//gf3d_matrix_perspective(camera->perspective, camera->fieldOfView, camera->width / camera->height, camera->nearPlane, camera->farPlane);
	//camera->perspective[1][1] *= -1.0f;
}

Camera *gf3d_camera_init(int renderWidth, int renderHeight, Vector3D *position, Vector3D *rotation)
{
	const Vector3D defaultUp = { 0.0f, 1.0f, 0.0f };
	const Vector3D defaultFront = { 0.0f, 0.0f, -1.0f };
	Camera *camera = NULL;

	camera = (Camera *)malloc(sizeof(Camera));
	if (!camera)
	{
		slog("Error: Could not allocate memory for the Camera.");
		return NULL;
	}
	memset(camera, 0, sizeof(Camera));

	camera->ubo = uniforms_get_local_reference(gf3d_vgraphics_get_uniform_buffer_manager(), 0, 0);
	
	//up
	memcpy(&camera->up, &defaultUp, sizeof(Vector3D));
	memcpy(&camera->worldUp, &defaultUp, sizeof(Vector3D));

	//internal position
	camera->position = position;
	camera->rotation = rotation;

	//front
	memcpy(&camera->front, &defaultFront, sizeof(Vector3D));

	camera->width = renderWidth;
	camera->height = renderHeight;
	camera->fieldOfView = 50.0f;
	camera->nearPlane = 0.01f;
	camera->farPlane = 100.0f;

	//gf3d_matrix_identity(camera->ubo->view);
	//gf3d_matrix_identity(camera->ubo->proj);

	//gf3d_matrix_view(camera->ubo->view, *camera->position, vector3d(0, 0, 0), camera->up);
	//gf3d_matrix_perspective(camera->ubo->proj, camera->fieldOfView, gf3d_camera_aspect_ratio(camera), camera->nearPlane, camera->farPlane);

	return camera;
}

/*eol@eof*/
