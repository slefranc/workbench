#ifndef __GIFTILABEL_H__
#define __GIFTILABEL_H__

/*LICENSE_START*/ 
/* 
 *  Copyright 1995-2002 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 


#include "CaretObject.h"
#include "TracksModificationInterface.h"

#include <stdint.h>

#include <QString>

namespace caret {

/**
 * Represents a GIFTI Label.
 */
class GiftiLabel : public CaretObject, TracksModificationInterface {

public:
    GiftiLabel(
                   const int32_t key,
                   const QString& name);

    explicit GiftiLabel(
                   const int32_t key,
                   const QString& name,
                   const float red,
                   const float green,
                   const float blue,
                   const float alpha);

    explicit GiftiLabel(
                        const int32_t key,
                        const QString& name,
                        const float red,
                        const float green,
                        const float blue,
                        const float alpha,
                        const float x,
                        const float y,
                        const float z);
    
    explicit GiftiLabel(
                        const int32_t key,
                        const QString& name,
                        const double red,
                        const double green,
                        const double blue,
                        const double alpha);
    
    GiftiLabel(
                   const int32_t key,
                   const QString& name,
                   const float rgba[]);

    explicit GiftiLabel(
                   const int32_t key,
                   const QString& name,
                   const int32_t red,
                   const int32_t green,
                   const int32_t blue,
                   const int32_t alpha);

    GiftiLabel(
                   const int32_t key,
                   const QString& name,
                   const int32_t rgba[]);

    GiftiLabel(const int32_t key);

    GiftiLabel(const GiftiLabel& gl);

public:
    GiftiLabel& operator=(const GiftiLabel& gl);

    virtual ~GiftiLabel();

private:
    void copyHelper(const GiftiLabel& o);

    void initializeMembersGiftiLabel();
    
public:
    int32_t hashCode();

    bool equals(const GiftiLabel&);

    int32_t operator<(const GiftiLabel& gl);

    int32_t getKey() const;

    void setKey(const int32_t key);

    QString getName() const;

    void setName(const QString& name);

    bool isSelected() const;

    void setSelected(const bool selected);

    float* getColor() const;

    void getColor(float rgbaOut[]) const;

    void setColor(const float rgba[]);

    int32_t* getColorInt() const;

    void setColorInt(const int32_t rgba[]);

    static void getDefaultColor(float rgbaOut[4]);

    float getRed() const;
    
    float getGreen() const;
    
    float getBlue() const;
    
    float getAlpha() const;

    float getX() const;
    
    float getY() const;
                        
    float getZ() const;
    
    void getXYZ(float xyz[3]) const;
    
    void setX(const float x);
    
    void setY(const float x);
    
    void setZ(const float x);
    
    void setXYZ(const float xyz[3]);
    
    void setModified();

    void clearModified();

    bool isModified() const;

    QString toString() const;

    int32_t getCount() const;

    void setCount(const int32_t count);

    void incrementCount();

private:
    /**tracks modification status (DO NOT CLONE) */
    bool modifiedFlag;

    QString name;

    int32_t key;

    bool selected;

    float red;

    float green;

    float blue;

    float alpha;

    float x;
    
    float y;
    
    float z;
    
    /**Used to count nodes/voxel using label (not saved in file) */
    int32_t count;

};

} // namespace

#endif // __GIFTILABEL_H__
