#ifndef __MESH__
#define __MESH__

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gf3d_types.h"
#include "gf3d_vector.h"
#include "gf3d_text.h"


enum meshDrawModes
{
	meshDrawWireframe = 1,
	meshDrawMesh = 2,
	meshDrawSmooth = 3,
	meshDrawTextured = 4,
	meshDrawLighting = 5,
	meshDrawShaded = 6
};

/**
 * @brief A vertex in 2D space, uses in texture space
 */
typedef float Vertex[2];

typedef struct face_s
{
	Sint32 vertices[3];
	Sint32 normals[3];
	Sint32 texels[3];
}Face;

typedef struct group_s
{
	TextLine name;
	Sint32 numVerts;
	Sint32 *vertices;
	float *weights;
	Sint32 _assignedV;
	Sint32 _assignedW;
}Group;

typedef struct mesh_s
{
	Uint8 refCount; /**<If space is in use, and the number of times the mesh appears in the game at once*/
	Uint32 id; /**<Internal ID of mesh*/

	TextLine filename; /**<Filename of mesh to load*/
	Vector3D center; /**<Relavtive position of the center of the mesh*/
	Vector3D bounds; /**<Width, height, and depth of the mesh*/

	Uint32 numVertices; /**<Number of vertices the mesh has*/
	Uint32 numNormals; /**<Number of normals calculated*/
	Uint32 numTexels; /**<Number of texture coordinates*/
	Uint32 numFaces; /**<Number of faces*/
	Uint32 numGroups; /**<Number of bones associated with this mesh*/

	Vector3D *rest; /**<Pointer to allocated vertex array at rest position*/
	Vector3D *normals; /**<Pointer to allocated normal array*/
	Vertex *texels; /**<Pointer to allocated texture vertex array*/
	Face *faces; /**<Pointer to allocated triangle array*/
	Group *groups; /**<Pointer to allocated bone array*/
	Vector3D *vertices; /**<Pointer to allocated vertex array*/
	Sint32 currFrame; /**<Frame that is currently in the vertices list*/

	Uint8 usesBuffer;
	Uint32 vertexArrayIndex;
	Uint32 normalArrayIndex;
	Uint32 texelArrayIndex;
}Mesh;

/**
 * @brief Closes the Mesh System
 */
void mesh_system_close();

/**
 * @brief Initializes the Mesh system
 * @param maxMeshes The maximum number of unique meshes that can be loaded at one time
 */
void mesh_system_init(Uint32 maxMeshes);

/**
 * @brief Checks if the Mesh system has been initialized
 * @returns 0 if Mesh system has not been initialized; 1 if it has
 */
Uint8 mesh_initialized();

/**
 * @brief Finds a spot in the Mesh manager that is not in use for a new Mesh
 * @returns Memory address where a new Mesh can be stored; NULL if no more space available
 */
Mesh * mesh_new();

/**
 * @brief Attempts to load a Mesh from a .obj file
 * @param filename The name of the .obj file to load
 * @returns A Mesh pointer to the new Mesh; NULL if failed
 */
Mesh * mesh_load_from_file(char *filename);

#endif
