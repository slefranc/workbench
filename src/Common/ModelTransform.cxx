
/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
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

#define __MODEL_TRANSFORM_DECLARE__
#include "ModelTransform.h"
#undef __MODEL_TRANSFORM_DECLARE__

#include <QStringList>

#include "CaretLogger.h"

using namespace caret;


    
/**
 * \class caret::ModelTransform 
 * \brief Translation, Rotation, and Scaling for a model.
 *
 * Translation, Rotation, and Scaling for a model.
 */
/**
 * Constructor.
 */
ModelTransform::ModelTransform()
: CaretObject()
{
    this->name = "";
    this->comment = "";
    
    setToIdentity();
}

/**
 * Destructor.
 */
ModelTransform::~ModelTransform()
{
    
}

/**
 * Set the view to the identity matrix.
 */
void
ModelTransform::setToIdentity()
{
    this->translation[0] = 0.0;
    this->translation[1] = 0.0;
    this->translation[2] = 0.0;
    
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            if (i == j) {
                this->rotation[i][j] = 1.0;
            }
            else {
                this->rotation[i][j] = 0.0;
            }
        }
    }
    
    this->scaling = 1.0;    
}

/**
 * Copy constructor.
 * @param ModelTransform
 *   View that is copied.
 */
ModelTransform::ModelTransform(const ModelTransform& ModelTransform)
: CaretObject(ModelTransform)
{
    this->copyHelper(ModelTransform);
}

/**
 * Assignment operator.
 * @param ModelTransform
 *    View that is copied to this view.
 * @return 
 *    Reference to this object.
 */
ModelTransform& 
ModelTransform::operator=(const ModelTransform& ModelTransform)
{
    if (this != &ModelTransform) {
        CaretObject::operator=(ModelTransform);
        this->copyHelper(ModelTransform);
    }
    
    return *this;
}

/**
 * Less than operator.
 * @param view
 *    View compared to this view.
 * @return
 *    Returns result of a name comparison.
 */
bool 
ModelTransform::operator<(const ModelTransform& view) const
{
    return (this->name < view.name);
}

/**
 * Equality operator.
 * @param view
 *    View compared to this view.
 * @return
 *    Returns true if views have same name.
 */
bool
ModelTransform::operator==(const ModelTransform& view) const
{
    return (this->name == view.name);
}

/**
 * Get the translation
 * @param translation
 *   Output translation.
 */
void 
ModelTransform::getTranslation(float translation[3]) const
{
    translation[0] = this->translation[0];
    translation[1] = this->translation[1];
    translation[2] = this->translation[2];
}

/**
 * Get the rotation matrix.
 * @param rotation
 *   Output rotation matrix.
 */
void 
ModelTransform::getRotation(float rotation[4][4]) const
{
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            rotation[i][j] = this->rotation[i][j];
        }
    }
}

/**
 * @return The scaling.
 */
float 
ModelTransform::getScaling() const
{
    return this->scaling;
}

/**
 * @return Name of the view.
 */
AString 
ModelTransform::getName() const
{
    return this->name;
}

/**
 * Set the name of the view.
 * @param name
 *   New name for view.
 */
void 
ModelTransform::setName(const AString& name)
{
    this->name = name;
}

/**
 * @return Comment of the view.
 */
AString
ModelTransform::getComment() const
{
    return this->comment;
}

/**
 * Set the comment of the view.
 * @param comment
 *   New comment for view.
 */
void
ModelTransform::setComment(const AString& comment)
{
    this->comment = comment;
}

/**
 * Set the translation
 * @param translation
 *   New translation.
 */
void 
ModelTransform::setTranslation(const float translation[3])
{
    this->translation[0] = translation[0];
    this->translation[1] = translation[1];
    this->translation[2] = translation[2];
}

/**
 * Set the translation
 * @param translationX
 *   New translation X-value.
 * @param translationY
 *   New translation Y-value.
 * @param translationZ
 *   New translation Z-value.
 */
void
ModelTransform::setTranslation(const float translationX,
                         const float translationY,
                         const float translationZ)
{
    this->translation[0] = translationX;
    this->translation[1] = translationY;
    this->translation[2] = translationZ;
}


/**
 * Set the rotation matrix.
 * @param rotation
 *   New rotation matrix.
 */
void 
ModelTransform::setRotation(const float rotation[4][4])
{
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            this->rotation[i][j] = rotation[i][j];
        }
    }
}

/**
 * Set the scaling
 * @param scaling
 *    New value for scaling.
 */
void 
ModelTransform::setScaling(const float scaling)
{
    this->scaling = scaling;
}

/**
 * Returns the user view in a string that contains,
 * separated by commas: View Name, translation[3],
 * rotation[4][4], and scaling.
 */
AString 
ModelTransform::getAsString() const
{
    AString s = (this->name
                 + s_separatorInPreferences + this->comment
                 + s_separatorInPreferences + AString::number(this->translation[0])
                 + s_separatorInPreferences + AString::number(this->translation[1])
                 + s_separatorInPreferences + AString::number(this->translation[2]));
    
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            s += (s_separatorInPreferences + AString::number(this->rotation[i][j]));
        }
    }
    
    s += (s_separatorInPreferences + AString::number(this->scaling));
    
    return s;
}

/**
 * Set the user view from a string that contains,
 * separated by commas: View Name, translation[3],
 * rotation[4][4], and scaling.
 */
bool 
ModelTransform::setFromString(const AString& s)
{
    bool hasComment = false;
    QStringList sl;
    if (s.contains(s_separatorInPreferences)) {
        sl = s.split(s_separatorInPreferences,
                                 QString::KeepEmptyParts);
        const int numElements = sl.count();
        if (numElements != 22) {
            CaretLogSevere("User view string does not contain 22 elements");
            return false;
        }
        hasComment = true;
    }
    else {
        sl = s.split(",", QString::KeepEmptyParts);
        const int numElements = sl.count();
        if (numElements != 21) {
            CaretLogSevere("User view string does not contain 21 elements");
            return false;
        }
    }
    
    int ctr = 0;
    this->name = sl.at(ctr++);
    if (hasComment) {
        this->comment = sl.at(ctr++);
    }
    else {
        this->comment = "";
    }
    this->translation[0] = sl.at(ctr++).toFloat();
    this->translation[1] = sl.at(ctr++).toFloat();
    this->translation[2] = sl.at(ctr++).toFloat();
    
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            this->rotation[i][j] = sl.at(ctr++).toFloat();
        }
    }
    
    this->scaling = sl.at(ctr++).toFloat();

    return true;
}

/**
 * Copy all data from the given user view to this user view.
 * @param ModelTransform
 *    View from which data is copied.
 */
void 
ModelTransform::copyHelper(const ModelTransform& modelTransform)
{
    this->name = modelTransform.name;
    this->comment = modelTransform.comment;
    
    this->translation[0] = modelTransform.translation[0];
    this->translation[1] = modelTransform.translation[1];
    this->translation[2] = modelTransform.translation[2];
    
    for (int32_t i = 0; i < 4; i++) {
        for (int32_t j = 0; j < 4; j++) {
            this->rotation[i][j] = modelTransform.rotation[i][j];
        }
    }
    
    this->scaling = modelTransform.scaling;
}

/**
 * Set panning (X, Y, & Z), rotation (X, Y, & Z), and zoom.
 *
 * @param panX
 *    X-Panning.
 * @param panY
 *    Y-Panning.
 * @param panZ
 *    Z-Panning.
 * @param rotationMatrix
 *    4x4 rotation matrix.
 * @param zoom
 *    Zooming.
 */
void
ModelTransform::setPanningRotationMatrixAndZoom(const float panX,
                                                const float panY,
                                                const float panZ,
                                                const float rotationMatrix[4][4],
                                                const float zoom)
{
    this->setTranslation(panX, panY, panZ);

    setRotation(rotationMatrix);
    
    this->setScaling(zoom);
}

/**
 * Get pan (X, Y, & Z), rotation (X, Y, & Z), and zoom.
 *
 * @param panX
 *    X-Panning.
 * @param panY
 *    Y-Panning.
 * @param rotationMatrix
 *    4x4 rotation matrix.
 * @param zoom
 *    Zooming.
 */
void
ModelTransform::getPanningRotationMatrixAndZoom(float& panX,
                                                float& panY,
                                                float& panZ,
                                                float rotationMatrix[4][4],
                                                float& zoom) const
{
    panX = this->translation[0];
    panY = this->translation[1];
    panZ = this->translation[2];
    
    getRotation(rotationMatrix);
    
    zoom = getScaling();
}


/**
 * Get a description of this object's content.
 * @return String describing this object's content.
 */
AString
ModelTransform::toString() const
{
    return ("ModelTransform: " + this->getAsString());
}
