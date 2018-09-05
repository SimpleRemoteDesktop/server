//
// Created by sylvain on 04/09/18.
//

#ifndef SIMPLEREMOTEDESKTOP_SERVER_ABSTRACTENCODER_H
#define SIMPLEREMOTEDESKTOP_SERVER_ABSTRACTENCODER_H


#include "../../config.h"
#include "../../Image.h"

class AbstractEncoder {
public:
    virtual SRD_Buffer_Frame* encode(Image* image) = 0;

};


#endif //SIMPLEREMOTEDESKTOP_SERVER_ABSTRACTENCODER_H
