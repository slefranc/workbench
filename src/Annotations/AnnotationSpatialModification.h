#ifndef __ANNOTATION_SPATIAL_MODIFICATION_H__
#define __ANNOTATION_SPATIAL_MODIFICATION_H__

/*LICENSE_START*/
/*
 *  Copyright (C) 2015 Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/


#include "AnnotationSizingHandleTypeEnum.h"
#include "CaretObject.h"



namespace caret {

    class AnnotationSpatialModification : public CaretObject {
        
    public:
        AnnotationSpatialModification(const AnnotationSizingHandleTypeEnum::Enum sizingHandleType,
                                      const float viewportWidth,
                                      const float viewportHeight,
                                      const float mousePressX,
                                      const float mousePressY,
                                      const float mouseX,
                                      const float mouseY,
                                      const float mouseDX,
                                      const float mouseDY);
        
        virtual ~AnnotationSpatialModification();
        

        // ADD_NEW_METHODS_HERE

        virtual AString toString() const;
        
    private:
        AnnotationSpatialModification(const AnnotationSpatialModification&);

        AnnotationSpatialModification& operator=(const AnnotationSpatialModification&);
        
        const AnnotationSizingHandleTypeEnum::Enum m_sizingHandleType;
        
        const float m_viewportWidth;
        
        const float m_viewportHeight;
        
        const float m_mousePressX;
        
        const float m_mousePressY;
        
        const float m_mouseX;
        
        const float m_mouseY;
        
        const float m_mouseDX;
        
        const float m_mouseDY;
        
        // ADD_NEW_MEMBERS_HERE

        friend class AnnotationOneDimensionalShape;
        friend class AnnotationText;
        friend class AnnotationTwoDimensionalShape;
    };
    
#ifdef __ANNOTATION_SPATIAL_MODIFICATION_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __ANNOTATION_SPATIAL_MODIFICATION_DECLARE__

} // namespace
#endif  //__ANNOTATION_SPATIAL_MODIFICATION_H__
