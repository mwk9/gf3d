#ifndef __GF3D_MODEL_H__
#define __GF3D_MODEL_H__

/**
 * gf3d_model
 * @license The MIT License (MIT)
   @copyright Copyright (c) 2015 EngineerOfLies
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "gf3d_types.h"
#include "gf3d_vector.h"
#include "gf3d_matrix.h"
#include "gf3d_text.h"
#include "gf3d_texture.h"
#include "gf3d_mesh.h"

typedef struct
{
    Uint8                       _inuse;
    Uint32                      _refcount;
    TextLine                    filename;
    Mesh                    *   mesh;
    Texture                 *   texture;
    VkDescriptorPool            descriptorPool;
    VkDescriptorSet         *   descriptorSets;
    Uint32                      descriptorSetCount;

}Model;


void gf3d_model_manager_init(Uint32 max_models,Uint32 chain_length,VkDevice device);
VkDescriptorSetLayout * gf3d_model_get_descriptor_set_layout();

Model * gf3d_model_load(char * filename);
Model * gf3d_model_new();
void gf3d_model_draw(Model *model,Uint32 bufferFrame);
void gf3d_model_free(Model *model);


#endif
