#ifndef __NIFTIDATATYPE_H__
#define __NIFTIDATATYPE_H__

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


#include "NiftiException.h"

#include <stdint.h>

#include <vector>
#include <QString>

namespace caret {

/**
 * NIFTI Data Types.  Note that only a small subset are used
 * in GIFTI data files.
 */
class NiftiDataTypeEnum {

public:
    /**  NIFTI Data Types.  Note that only a small subset are used
 in GIFTI data files.
 */
    enum Enum {
        /** invalid data type.  */
        NIFTI_TYPE_INVALID,
        /** unsigned byte.  */
        NIFTI_TYPE_UINT8,
        /** signed short.  */
        NIFTI_TYPE_INT16,
        /** signed int.  */
        NIFTI_TYPE_INT32,
        /** 32 bit float.  */
        NIFTI_TYPE_FLOAT32,
        /** 64 bit complex = 2 32 bit floats.  */
        NIFTI_TYPE_COMPLEX64,
        /** 64 bit float = double.  */
        NIFTI_TYPE_FLOAT64,
        /** 3 8 bit bytes.  */
        NIFTI_TYPE_RGB24,
        /** signed char.  */
        NIFTI_TYPE_INT8,
        /** unsigned short.  */
        NIFTI_TYPE_UINT16,
        /** unsigned int.  */
        NIFTI_TYPE_UINT32,
        /** signed long long.  */
        NIFTI_TYPE_INT64,
        /** unsigned long long.  */
        NIFTI_TYPE_UINT64,
        /** 128 bit float = long double.  */
        NIFTI_TYPE_FLOAT128,
        /** 128 bit complex = 2 64 bit floats.  */
        NIFTI_TYPE_COMPLEX128,
        /** 256 bit complex = 2 128 bit floats  */
        NIFTI_TYPE_COMPLEX256
    };


    ~NiftiDataTypeEnum();
private:
    NiftiDataTypeEnum();
    NiftiDataTypeEnum(Enum e, const QString& name, const int32_t integerCode);
    static const NiftiDataTypeEnum* findData(Enum e);
    static std::vector<NiftiDataTypeEnum> dataTypes;
    static void createDataTypes();
    static bool dataTypesCreatedFlag;
    
    Enum e;
    QString name;
    int32_t integerCode;
    
public:
    static QString toName(Enum e);
    static Enum fromName(const QString& s, bool* isValidOut);
    static int32_t toIntegerCode(Enum e);
    static Enum fromIntegerCode(const int32_t integerCode, bool* isValidOut);

};

#ifdef __NIFTI_DATA_TYPE_DECLARE__
    std::vector<NiftiDataTypeEnum> NiftiDataTypeEnum::dataTypes;
    bool NiftiDataTypeEnum::dataTypesCreatedFlag = false;
#endif // __NIFTI_DATA_TYPE_DECLARE__
} // namespace

#endif // __NIFTIDATATYPE_H__
