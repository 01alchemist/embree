// ======================================================================== //
// Copyright 2009-2015 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#if USE_LIBPNG
#include "texture_loader.h"
#include "image/image.h"

#include <png.h>

namespace embree
{
  /*! read png texture from disk */
  OBJScene::Texture *loadTexture(const FileName& fileName)
  {
    OBJScene::Texture *texture = new OBJScene::Texture();
    
    Ref<Image> img = loadImage(fileName);

    texture->width         = img.ptr->width;
    texture->height        = img.ptr->height;    
    texture->format        = OBJScene::Texture::RGBA8;
    texture->bytesPerTexel = 4;
    texture->data          = _mm_malloc(sizeof(int)*texture->width*texture->height,64);
    img.ptr->convertToRGBA8((unsigned char*)texture->data);
    return texture;
  }
}
#endif
