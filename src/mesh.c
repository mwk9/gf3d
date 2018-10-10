#include "mesh.h"
#include "simple_logger.h"

//local global variables
Uint32 meshShaderProgram = 0;
Uint8 meshSystemInitialized = 0;
typedef struct meshManager_s
{
	Uint32 maxMeshes;
	MyMesh *meshList;
	Uint64 increment;
}MeshManager;

static MeshManager meshManager = { 0, NULL, 0 };

void mesh_system_close()
{
	int i = 0;

	if (meshManager.meshList != NULL)
	{
		for (i = 0; i < meshManager.maxMeshes; i++)
		{
			//mesh_free
		}
	}
}

void mesh_system_init(Uint32 maxMeshes)
{
	if (maxMeshes <= 0)
	{
		slog("Error: Cannot initialize Mesh manager for zero or negative meshes");
		return;
	}

	memset(&meshManager, 0, sizeof(MeshManager));
	meshManager.meshList = (MyMesh *)malloc(sizeof(MyMesh) * maxMeshes);
	if (!meshManager.meshList)
	{
		slog("Error: Could not allocate memory of the mesh list");
		mesh_system_close();
		return;
	}
	memset(meshManager.meshList, 0, sizeof(MyMesh) * maxMeshes);
	meshManager.maxMeshes = maxMeshes;
	meshSystemInitialized = 1;

	slog("Mesh system initialized");
	atexit(mesh_system_close);
}

Uint8 mesh_initialized()
{
	return meshSystemInitialized;
}

MyMesh * mesh_new()
{
	int i = 0;

	for (i = 0; i < meshManager.maxMeshes; i++)
	{
		if (meshManager.meshList[i].refCount == 0)
		{
			slog("Found a spot for a new mesh at index (%i)", i);
			memset(&meshManager.meshList[i], 0, sizeof(MyMesh));
			meshManager.meshList[i].id = meshManager.increment++;
			meshManager.meshList[i].refCount = 1;
			return &meshManager.meshList[i];
		}
	}

	slog("Error: Out of mesh addresses for a new mesh");
	return NULL;
}

/**
 * @brief Gets the number of vertices, normals, texture coordinates, and faces from a file and applies it to a given mesh
 */
void mesh_object_get_counts(FILE *file, MyMesh *m)
{
	char buffer[128];
	int numVertices = 0;
	int numNormals = 0;
	int numTexcoords = 0;
	int numFaces = 0;

	if (file == NULL || m == NULL)
	{
		slog("Error: file or m was NULL");
		return;
	}

	while (fscanf(file, "%s", buffer) != EOF)
	{
		switch (buffer[0])
		{
			case 'v':
				switch (buffer[1])
				{
					case '\0':
						fgets(buffer, sizeof(buffer), file);
						numVertices++;
						break;
					case 'n':
						fgets(buffer, sizeof(buffer), file);
						numNormals++;
						break;
					case 't':
						fgets(buffer, sizeof(buffer), file);
						numTexcoords++;
						break;
					default:
						break;
				}
				break;
			case 'f':
				fgets(buffer, sizeof(buffer), file);
				numFaces++;
				break;
			default:
				fgets(buffer, sizeof(buffer), file);
				break;
		}
	}

	m->numVertices = numVertices;
	m->numNormals = numNormals;
	m->numTexels = numTexcoords;
	m->numFaces = numFaces;
}

void mesh_get_mesh_from_object_file(FILE *file, MyMesh *m)
{
	int numVertices = 0;
	int numNormals = 0;
	int numTexcoords = 0;
	int numFaces = 0;
	char buffer[512];
	Vector3D bounds = { 0, 0, 0 };
	float x, y, z;

	if (file == NULL)
	{
		return;
	}
	
	rewind(file);
	while (fscanf(file, "%s", buffer) != EOF)
	{
		switch (buffer[0])
		{
			case 'v':
				switch (buffer[1])
				{
					case '\0':
						fscanf(file, "%f %f %f", &x, &y, &z);
						m->vertices[numVertices].x = x;
						m->vertices[numVertices].y = y;
						m->vertices[numVertices].z = z;
						if (fabs(x) > bounds.x) { bounds.x = fabs(x); }
						if (fabs(y) > bounds.y) { bounds.y = fabs(y); }
						if (fabs(z) > bounds.z) { bounds.z = fabs(z); }
						numVertices++;
						break;
					case 'n':
						fscanf(file, "%f %f %f", &x, &y, &z);
						m->normals[numNormals].x = x;
						m->normals[numNormals].y = y;
						m->normals[numNormals].z = z;
						numNormals++;
						break;
					case 't':
						fscanf(file, "%f %f", &m->texels[numTexcoords][0], &m->texels[numTexcoords][1]);
						m->texels[numTexcoords][1] = 1 - m->texels[numTexcoords][1];
						numTexcoords++;
						break;
					default:
						break;
				}
				break;
			case 'f':
				fscanf
				(
					file,
					"%d/%d/%d %d/%d/%d %d/%d/%d",
					&m->faces[numFaces].vertices[0],
					&m->faces[numFaces].texels[0],
					&m->faces[numFaces].normals[0],
					&m->faces[numFaces].vertices[1],
					&m->faces[numFaces].texels[1],
					&m->faces[numFaces].normals[1],
					&m->faces[numFaces].vertices[2],
					&m->faces[numFaces].texels[2],
					&m->faces[numFaces].normals[2]
				);
				m->faces[numFaces].vertices[0]--;
				m->faces[numFaces].texels[0]--;
				m->faces[numFaces].normals[0]--;
				m->faces[numFaces].vertices[1]--;
				m->faces[numFaces].texels[1]--;
				m->faces[numFaces].normals[1]--;
				m->faces[numFaces].vertices[2]--;
				m->faces[numFaces].texels[2]--;
				m->faces[numFaces].normals[2]--;
				numFaces++;
				break;
			default:
				fgets(buffer, sizeof(buffer), file);
				break;
		}
	}
	vector3d_copy(m->bounds, bounds);
}

MyGroup * mesh_get_group_by_name(char *name, MyMesh *m)
{
	int i = 0;
	if (name == NULL || m == NULL)
	{
		return NULL;
	}

	for (i = 0; i < m->numGroups; i++)
	{
		if (strcmp(m->groups[i].name, name) == 0)
		{
			return &m->groups[i];
		}
	}

	slog("Warning: Unable to find group (%s) in mesh (%s)", name, m->filename);
	return NULL;
}

void mesh_get_groups(FILE *file, MyMesh *m)
{
	Uint8 badLoad = 0;
	TextLine buffer;
	TextLine name;
	Uint32 numGroups = 0;
	Uint32 i = 0;
	Uint32 g = 0;
	float weight = 0.0f;
	MyGroup *group = NULL;

	if (file == NULL || m == NULL)
	{
		slog("Error: file or m was NULL");
		return;
	}

	rewind(file);
	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "vgroupcount:") == 0)
		{
			fscanf(file, "%i", &numGroups);
			break;
		}
	}
	//check if model has any bones to handle
	if (numGroups == 0)
	{
		return;
	}
	m->numGroups = numGroups;
	m->groups = (MyGroup *)malloc(sizeof(MyGroup) * numGroups);
	if (m->groups == NULL)
	{
		slog("Error: Could not allocate memory for mesh groups");
		free(m->groups);
		m->groups = NULL;
		m->numGroups = 0;
		return;
	}
	memset(m->groups, 0, sizeof(MyGroup) * numGroups);

	//name all of the groups
	rewind(file);
	g = 0;
	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "vgroup:") == 0)
		{
			fscanf(file, "%s", m->groups[g].name);
			g++;
			continue;
		}
		if (strcmp(buffer, "groupname:") == 0)
		{
			fscanf(file, "%s", name);
			group = mesh_get_group_by_name(name, m);
			if (group != NULL)
			{
				group->numVerts++;
			}
		}
	}

	//vertex and weight info for each group
	for (i = 0; i < g; i++)
	{
		m->groups[i].vertices = (Uint32 *)malloc(sizeof(Uint32) * (m->groups[i].numVerts));
		m->groups[i].weights = (float *)malloc(sizeof(float) * (m->groups[i].numVerts));
		if (m->groups[i].vertices == NULL || m->groups[i].weights == NULL)
		{
			badLoad = 1;
			break;
		}
		memset(m->groups[i].vertices, 0, sizeof(Uint32) * (m->groups[i].numVerts));
		memset(m->groups[i].weights, 0, sizeof(float) * (m->groups[i].numVerts));
	}
	if (badLoad > 0)
	{
		slog("Error: Unable to allocate group vertices/weights");
		for (i = 0; i < g; i++)
		{
			if (m->groups[i].vertices != NULL) { free(m->groups[i].vertices); }
			if (m->groups[i].weights != NULL) { free(m->groups[i].weights); }
		}
		free(m->groups);
		m->groups = NULL;
		m->numGroups = 0;
	}

	//assign information
	rewind(file);
	g = 0;
	i = -1;
	group = NULL;
	while (fscanf(file, "%s", buffer) != EOF)
	{
		if (strcmp(buffer, "groupname:") == 0)
		{
			fscanf(file, "%s", name);
			group = mesh_get_group_by_name(name, m);
			if (group != NULL)
			{
				group->vertices[group->_assignedV] = i;
				group->_assignedV++;
			}
			continue;
		}
		if (strcmp(buffer, "weight:") == 0)
		{
			if (group != NULL)
			{
				fscanf(file, "%f", &weight);
				group->weights[group->_assignedW] = weight;
				group->_assignedW++;
			}
			continue;
		}
		if (buffer[0] == 'v' && buffer[1] == '\0')
		{
			i++;
			continue;
		}
	}
}

/**
 * @brief Loads mesh data from object file and stores the information into the provided Mesh
 */
void mesh_load_from_object_file(char *filename, MyMesh *m)
{
	FILE *file;
	file = fopen(filename, "r");

	if (!file)
	{
		slog("Error: Could not open file (%s) for mesh loading", filename);
		return;
	}

	strcpy(m->filename, filename);
	mesh_object_get_counts(file, m);

	//allocate enough memory for mesh details
	if (m->numVertices != 0)
	{
		m->vertices = (Vector3D *)malloc(sizeof(Vector3D) * (m->numVertices));
		memset(m->vertices, 0, sizeof(Vector3D) * (m->numVertices));
		m->rest = (Vector3D *)malloc(sizeof(Vector3D) * (m->numVertices));
		memset(m->rest, 0, sizeof(Vector3D) * (m->numVertices));
	}
	if (m->numNormals != 0)
	{
		m->normals = (Vector3D *)malloc(sizeof(Vector3D) * (m->numNormals));
		memset(m->normals, 0, sizeof(Vector3D) * (m->numNormals));
	}
	if (m->numTexels != 0)
	{
		m->texels = (MyVertex *)malloc(sizeof(MyVertex) * (m->numTexels));
		memset(m->texels, 0, sizeof(MyVertex) * (m->numTexels));
	}
	if (m->numFaces != 0)
	{
		m->faces = (MyFace *)malloc(sizeof(MyFace) * (m->numFaces));
		memset(m->faces, 0, sizeof(MyFace) * (m->numFaces));
	}
	mesh_get_mesh_from_object_file(file, m);
	memcpy(m->rest, m->vertices, sizeof(Vector3D) * (m->numVertices));
	m->currFrame = -1;
	mesh_get_groups(file, m);
	fclose(file);
}

/**
 * @brief Searches through the Mesh list to see if mesh has already been loaded
 * @param filename The filename of the Mesh to search for
 * @returns A reference to the Mesh if it already exists in the Mesh list; NULL if it does not
 */
MyMesh * mesh_get_by_filename(char *filename)
{
	int i = 0;

	if (!mesh_initialized())
	{
		return NULL;
	}
	if (filename == NULL)
	{
		slog("Error: Trying to load a mesh from a NULL filename");
		return NULL;
	}

	for (i = 0; i < meshManager.maxMeshes; i++)
	{
		if (meshManager.meshList[i].refCount > 0)
		{
			if (strcmp(meshManager.meshList[i].filename, filename) == 0)
			{
				meshManager.meshList[i].refCount++;
				return &meshManager.meshList[i];
			}
		}
	}

	//Mesh is not currently loaded in the Mesh list
	return NULL;
}

MyMesh * mesh_load_from_file(char *filename)
{
	MyMesh *mesh = NULL;

	if (!mesh_initialized())
	{
		slog("Error: Trying to load a mesh from file, but mesh system not initialized yet");
		return NULL;
	}
	
	mesh = mesh_get_by_filename(filename);
	if (mesh != NULL)
	{
		//Mesh is already loaded, return a reference to it
		return mesh;
	}
	//Mesh is not yet loaded, load it now
	mesh = mesh_new();
	if (!mesh)
	{
		return NULL;
	}
	//load from file
	return mesh;
}
