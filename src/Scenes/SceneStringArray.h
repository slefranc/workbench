#ifndef __SCENE_STRING_ARRAY_H__
#define __SCENE_STRING_ARRAY_H__

/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
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


#include "ScenePrimitiveArray.h"

namespace caret {

    class SceneStringArray : public ScenePrimitiveArray {
        
    public:
        SceneStringArray(const AString& name,
                               const AString values[],
                               const int32_t numberOfArrayElements);
        
        SceneStringArray(const AString& name,
                          const std::vector<AString>& values);
        
        SceneStringArray(const AString& name,
                          const int numberOfArrayElements);
        
        SceneStringArray(const SceneStringArray& rhs);

        virtual ~SceneStringArray();
        
        void setValue(const int32_t arrayIndex,
                      const AString value);
        
        virtual bool booleanValue(const int32_t arrayIndex) const;
        
        virtual float floatValue(const int32_t arrayIndex) const;
        
        virtual int32_t integerValue(const int32_t arrayIndex) const;
        
        virtual AString stringValue(const int32_t arrayIndex) const;
        
        virtual uint8_t unsignedByteValue(const int32_t arrayIndex) const;
        
    private:
        SceneStringArray& operator=(const SceneStringArray&);
        
    public:
        
        virtual int32_t getNumberOfArrayElements() const { return m_values.size(); }
        
        virtual SceneObject* clone() const { return new SceneStringArray(*this); }

        // ADD_NEW_METHODS_HERE

    private:

        std::vector<AString> m_values;
        
        // ADD_NEW_MEMBERS_HERE

    };
    
#ifdef __SCENE_STRING_ARRAY_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __SCENE_STRING_ARRAY_DECLARE__

} // namespace
#endif  //__SCENE_STRING_ARRAY_H__
